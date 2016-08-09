#include <redstrain/util/Unref.hpp>
#include <redstrain/text/UTF8Encoder.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/DefaultCodecFactory.hpp>
#include <redstrain/error/UnsupportedPlatformFeatureError.hpp>
#include <redstrain/text/l10n.hpp>
#include <redstrain/redmond/environment.hpp>

#if defined(REDSTRAIN_ENV_OS_LINUX)
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

#include "UNIXTerminalBinding.hpp"

using std::string;
using redengine::util::Unref;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::text::Encoder16;
using redengine::text::Decoder16;
using redengine::text::CodecManager;
using redengine::text::CodecFactory;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF8Decoder16;
using redengine::text::DefaultCodecFactory;
using redengine::error::UnsupportedPlatformFeatureError;
using redengine::text::guessTerminalCharset;

#define NOT_UNIX throw UnsupportedPlatformFeatureError("UNIXTerminalBinding");

namespace redengine {
namespace damnation {

	UNIXTerminalBinding::UNIXTerminalBinding(const TermSpec& spec) : readfd(0), writefd(1), spec(spec),
			codecManager(NULL), encoderFactory(NULL), decoderFactory(NULL), oldTermios(NULL), usingUTF8(false) {
#if defined(REDSTRAIN_ENV_OS_LINUX)
		setEncoding(guessTerminalCharset());
		catchSIGWINCH();
#else /* non-UNIX */
		NOT_UNIX
#endif /* UNIX? */
	}

	UNIXTerminalBinding::UNIXTerminalBinding(const UNIXTerminalBinding& binding) : TerminalBinding(binding),
			readfd(binding.readfd), writefd(binding.writefd), spec(binding.spec), codecManager(binding.codecManager),
			encname(binding.encname), encoderFactory(binding.encoderFactory),
			decoderFactory(binding.decoderFactory), oldTermios(NULL), usingUTF8(binding.usingUTF8) {
#if defined(REDSTRAIN_ENV_OS_LINUX)
		if(encoderFactory)
			encoderFactory->ref();
		if(decoderFactory)
			decoderFactory->ref();
		if(binding.oldTermios)
			oldTermios = new struct termios(*reinterpret_cast<struct termios*>(binding.oldTermios));
#else /* non-UNIX */
		NOT_UNIX
#endif /* UNIX? */
	}

	UNIXTerminalBinding::~UNIXTerminalBinding() {
		if(encoderFactory)
			encoderFactory->unref();
		if(decoderFactory)
			decoderFactory->unref();
#if defined(REDSTRAIN_ENV_OS_LINUX)
		if(oldTermios)
			delete reinterpret_cast<struct termios*>(oldTermios);
#endif /* UNIX? */
	}

	void UNIXTerminalBinding::setEncoding(const string& encoding) {
		if(encoding.empty())
			encname = guessTerminalCharset();
		else
			encname = encoding;
		setEncoderFactoryFromEncoding();
		setDecoderFactoryFromEncoding();
	}

	void UNIXTerminalBinding::setEncoderFactory(CodecFactory<Encoder16>* factory) {
		if(factory) {
			factory->ref();
			if(encoderFactory)
				encoderFactory->unref();
			encoderFactory = factory;
		}
		else
			setEncoderFactoryFromEncoding();
	}

	void UNIXTerminalBinding::setDecoderFactory(CodecFactory<Decoder16>* factory) {
		if(factory) {
			factory->ref();
			if(decoderFactory)
				decoderFactory->unref();
			decoderFactory = factory;
		}
		else
			setDecoderFactoryFromEncoding();
	}

#define GET_CM (codecManager ? *codecManager : CodecManager::getDefaultCodecManager())

	void UNIXTerminalBinding::setEncoderFactoryFromEncoding() {
		Unref<CodecFactory<Encoder16> > newFactory;
		if(encname.empty())
			newFactory = new DefaultCodecFactory<Encoder16, UTF8Encoder16>;
		else {
			newFactory = GET_CM.loadEncoder16Factory(encname);
			if(*newFactory)
				newFactory->ref();
			else
				newFactory = new DefaultCodecFactory<Encoder16, UTF8Encoder16>;
		}
		if(encoderFactory)
			encoderFactory->unref();
		encoderFactory = *newFactory;
		encoderFactory->ref();
	}

	void UNIXTerminalBinding::setDecoderFactoryFromEncoding() {
		Unref<CodecFactory<Decoder16> > newFactory;
		if(encname.empty())
			newFactory = new DefaultCodecFactory<Decoder16, UTF8Decoder16>;
		else {
			newFactory = GET_CM.loadDecoder16Factory(encname);
			if(*newFactory)
				newFactory->ref();
			else
				newFactory = new DefaultCodecFactory<Decoder16, UTF8Decoder16>;
		}
		if(decoderFactory)
			decoderFactory->unref();
		decoderFactory = *newFactory;
		decoderFactory->ref();
	}

#if defined(REDSTRAIN_ENV_OS_LINUX)

	static volatile bool caughtWinch = false;

	static void winchHandler(int signal) {
		if(signal == SIGWINCH)
			caughtWinch = true;
	}

	void UNIXTerminalBinding::catchSIGWINCH() {
		struct sigaction action;
		action.sa_handler = winchHandler;
		if(sigemptyset(&action.sa_mask))
			return;
		action.sa_flags = 0;
		sigaction(SIGWINCH, &action, NULL);
	}

	static bool getattr(int fd, struct termios& attributes) {
		for(;;) {
			if(!tcgetattr(fd, &attributes))
				return true;
			if(errno != EINTR)
				return false;
		}
	}

	static bool setattr(int fd, int actions, const struct termios& attributes) {
		for(;;) {
			if(!tcsetattr(fd, actions, &attributes))
				return true;
			if(errno != EINTR)
				return false;
		}
	}

	TerminalBinding::InputMode UNIXTerminalBinding::getInputMode() {
		struct termios state;
		if(!getattr(readfd, state))
			return oldTermios ? IM_RAW : IM_COOKED;
		return state.c_lflag & ICANON ? IM_COOKED : IM_RAW;
	}

	void UNIXTerminalBinding::setRaw() {
		struct termios currentState;
		if(!getattr(readfd, currentState))
			return;
		if(!oldTermios)
			oldTermios = new struct termios(currentState);
#define set(type, flags) (currentState.c_ ## type ## flag |= (flags))
#define clear(type, flags) (currentState.c_ ## type ## flag &= ~(flags))
		set(i, IGNBRK);
		clear(i, PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IUCLC | IXON | IXOFF);
		if(usingUTF8)
			set(i, IUTF8);
		else
			clear(i, IUTF8);
		set(o, OPOST);
		clear(o, OLCUC);
		set(l, IEXTEN);
		clear(l, ISIG | ICANON | ECHO);
#undef set
#undef clear
		currentState.c_cc[VTIME] = (cc_t)0;
		currentState.c_cc[VMIN] = (cc_t)1;
		setattr(readfd, TCSADRAIN, currentState);
	}

	void UNIXTerminalBinding::setCooked() {
		if(!oldTermios)
			return;
		struct termios* ot = reinterpret_cast<struct termios*>(oldTermios);
		setattr(readfd, TCSADRAIN, *ot);
		delete ot;
		oldTermios = NULL;
	}

	void UNIXTerminalBinding::setInputMode(InputMode mode) {
		if(mode == IM_RAW)
			setRaw();
		else
			setCooked();
	}

	bool UNIXTerminalBinding::hasSizeChanged() {
		//TODO
		return false;

	}

	Size UNIXTerminalBinding::getSize() {
		//TODO
		return Size(0u, 0u);
	}

	bool UNIXTerminalBinding::supportsOperation(OptionalOperation) {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::carriageReturn() {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::newLine() {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::clearScreen() {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::moveTo(unsigned, unsigned) {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::moveBy(int, int) {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::cursorDown() {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::cursorDownBy(unsigned) {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::cursorUp() {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::cursorUpBy(unsigned) {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::cursorLeft() {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::cursorLeftBy(unsigned) {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::cursorRight() {
		//TODO
		return false;
	}

	bool UNIXTerminalBinding::cursorRightBy(unsigned) {
		//TODO
		return false;
	}

	unsigned UNIXTerminalBinding::getColorCount() {
		//TODO
		return 0u;
	}

	unsigned UNIXTerminalBinding::setForegroundColor(unsigned) {
		//TODO
		return 0u;
	}

	unsigned UNIXTerminalBinding::setBackgroundColor(unsigned) {
		//TODO
		return 0u;
	}

	int UNIXTerminalBinding::setTextAttributes(int) {
		//TODO
		return 0;
	}

	int UNIXTerminalBinding::addTextAttributes(int) {
		//TODO
		return 0;
	}

	int UNIXTerminalBinding::removeTextAttributes(int) {
		//TODO
		return 0;
	}

	void UNIXTerminalBinding::resetTextAttributes(int) {
		//TODO
	}

	KeySym UNIXTerminalBinding::read(bool) {
		//TODO
		return KeySym::NONE;
	}

	void UNIXTerminalBinding::write(char) {
		//TODO
	}

	void UNIXTerminalBinding::write(const string&) {
		//TODO
	}

	void UNIXTerminalBinding::write(Char16) {
		//TODO
	}

	void UNIXTerminalBinding::write(const String16&) {
		//TODO
	}

	void UNIXTerminalBinding::write(BoxSymbol) {
		//TODO
	}

	void UNIXTerminalBinding::flush() {
		//TODO
	}

#else /* non-UNIX */

	void UNIXTerminalBinding::catchSIGWINCH() {}
	void UNIXTerminalBinding::setRaw() {}
	void UNIXTerminalBinding::setCooked() {}

	TerminalBinding::InputMode UNIXTerminalBinding::getInputMode() {
		NOT_UNIX
	}

	void UNIXTerminalBinding::setInputMode(InputMode) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::hasSizeChanged() {
		NOT_UNIX

	}

	Size UNIXTerminalBinding::getSize() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::supportsOperation(OptionalOperation) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::carriageReturn() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::newLine() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::clearScreen() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::moveTo(unsigned, unsigned) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::moveBy(int, int) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::cursorDown() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::cursorDownBy(unsigned) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::cursorUp() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::cursorUpBy(unsigned) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::cursorLeft() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::cursorLeftBy(unsigned) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::cursorRight() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::cursorRightBy(unsigned) {
		NOT_UNIX
	}

	unsigned UNIXTerminalBinding::getColorCount() {
		NOT_UNIX
	}

	unsigned UNIXTerminalBinding::setForegroundColor(unsigned) {
		NOT_UNIX
	}

	unsigned UNIXTerminalBinding::setBackgroundColor(unsigned) {
		NOT_UNIX
	}

	int UNIXTerminalBinding::setTextAttributes(int) {
		NOT_UNIX
	}

	int UNIXTerminalBinding::addTextAttributes(int) {
		NOT_UNIX
	}

	int UNIXTerminalBinding::removeTextAttributes(int) {
		NOT_UNIX
	}

	void UNIXTerminalBinding::resetTextAttributes(int) {
		NOT_UNIX
	}

	KeySym UNIXTerminalBinding::read(bool) {
		NOT_UNIX
	}

	void UNIXTerminalBinding::write(char) {
		NOT_UNIX
	}

	void UNIXTerminalBinding::write(const string&) {
		NOT_UNIX
	}

	void UNIXTerminalBinding::write(Char16) {
		NOT_UNIX
	}

	void UNIXTerminalBinding::write(const String16&) {
		NOT_UNIX
	}

	void UNIXTerminalBinding::write(BoxSymbol) {
		NOT_UNIX
	}

	void UNIXTerminalBinding::flush() {
		NOT_UNIX
	}

#endif /* UNIX? */

}}
