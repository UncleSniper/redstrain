#include <cstring>
#include <sstream>
#include <iostream>
#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/platform/Thread.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/text/UTF8Encoder.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/util/DeleteArray.hpp>
#include <redstrain/platform/FileIOError.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/text/DefaultCodecFactory.hpp>
#include <redstrain/text/UnrepresentableCharacterError.hpp>
#include <redstrain/error/UnsupportedPlatformFeatureError.hpp>
#include <redstrain/text/l10n.hpp>
#include <redstrain/platform/utils.hpp>
#include <redstrain/redmond/environment.hpp>

#if defined(REDSTRAIN_ENV_OS_LINUX)
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

#include "ColorMap.hpp"
#include "UNIXTerminalBinding.hpp"
#include "IllegalColorIndexError.hpp"
#include "IllegalUNIXTerminalWriteModeError.hpp"
#include "unixutils/MappingSingleByteConverter.hpp"
#include "unixutils/UNIXTerminalBindingInputConverter.hpp"
#include "tparm.hpp"
#include "tweaks.hpp"

using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::platform::File;
using redengine::text::Encoder16;
using redengine::text::Decoder16;
using redengine::text::Transcode;
using redengine::platform::Thread;
using redengine::util::MemorySize;
using redengine::util::DeleteArray;
using redengine::util::StringUtils;
using redengine::text::CodecManager;
using redengine::text::CodecFactory;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF8Decoder16;
using redengine::platform::FileIOError;
using redengine::error::ProgrammingError;
using redengine::text::DefaultCodecFactory;
using redengine::text::UnrepresentableCharacterError;
using redengine::error::UnsupportedPlatformFeatureError;
using redengine::damnation::unixutils::SequenceMapInputConverter;
using redengine::damnation::unixutils::MappingSingleByteConverter;
using redengine::damnation::unixutils::UNIXTerminalBindingInputConverter;
using redengine::text::guessTerminalCharset;
using redengine::platform::currentTimeMillis;

#define NOT_UNIX throw UnsupportedPlatformFeatureError("UNIXTerminalBinding");

namespace redengine {
namespace damnation {

	UNIXTerminalBinding::UNIXTerminalBinding(const TermSpec& spec) : readfd(0), writefd(1), spec(spec),
			codecManager(NULL), encoderFactory(NULL), decoderFactory(NULL), oldTermios(NULL), usingUTF8(false),
			prevWidth(0u), prevHeight(0u), colorMode(UNIXTerminalBinding::colorModeFromColorCount(spec.max_colors)),
			currentAttributes(0), writeMode(WM_DIRECT), writeBlock(NULL), blockFill(static_cast<MemorySize>(0u)),
			inCAMode(false), specialKeyConverter(specialKeyMap), currentlyBlocking(false),
			specialKeyFlushDelay(static_cast<uint64_t>(
			REDSTRAIN_DAMNATION_UNIXTERMINALBINDING_SPECIAL_KEY_FLUSH_DELAY)) {
#if defined(REDSTRAIN_ENV_OS_LINUX)
		setEncoding(guessTerminalCharset());
		catchSIGWINCH();
#else /* non-UNIX */
		NOT_UNIX
#endif /* UNIX? */
		inputConverter.addSingleByteConverter(MappingSingleByteConverter::getControlKeysConverter());
		inputConverter.addSingleByteConverter(MappingSingleByteConverter::getAlternateBackspaceConverter());
		Delete<UNIXTerminalBindingInputConverter> decinconv(new UNIXTerminalBindingInputConverter(*this));
		inputConverter.addInputConverter(**decinconv);
		decoderInputConverter = *decinconv;
		decinconv.set();
		registerSpecialKeys();
		inputConverter.addInputConverter(specialKeyConverter);
	}

	UNIXTerminalBinding::UNIXTerminalBinding(const UNIXTerminalBinding& binding) : TerminalBinding(binding),
			readfd(binding.readfd), writefd(binding.writefd), spec(binding.spec), codecManager(binding.codecManager),
			encname(binding.encname), encoderFactory(binding.encoderFactory),
			decoderFactory(binding.decoderFactory), oldTermios(NULL), usingUTF8(binding.usingUTF8),
			prevWidth(binding.prevWidth), prevHeight(binding.prevHeight), colorMode(binding.colorMode),
			currentAttributes(binding.currentAttributes), writeMode(binding.writeMode),
			writeBlock(writeMode == WM_BLOCKED ? new char[REDSTRAIN_DAMNATION_UNIXTERMINALBINDING_BLOCK_SIZE]
			: NULL), blockFill(static_cast<MemorySize>(0u)), inCAMode(binding.inCAMode),
			inputConverter(binding.inputConverter), specialKeyMap(binding.specialKeyMap),
			specialKeyConverter(specialKeyMap), currentlyBlocking(binding.currentlyBlocking),
			specialKeyFlushDelay(binding.specialKeyFlushDelay) {
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
		inputConverter.removeInputConverter(*binding.decoderInputConverter);
		Delete<UNIXTerminalBindingInputConverter> decinconv(new UNIXTerminalBindingInputConverter(*this));
		inputConverter.addInputConverter(**decinconv);
		decoderInputConverter = decinconv.set();
		inputConverter.removeInputConverter(binding.specialKeyConverter);
		inputConverter.addInputConverter(specialKeyConverter);
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
		if(writeBlock)
			delete[] writeBlock;
	}

	UNIXTerminalBinding::ColorMode UNIXTerminalBinding::colorModeFromColorCount(uint32_t count) {
		if(count >= static_cast<uint32_t>(120u))
			return CM_HIGH_COLOR;
		else if(count >= static_cast<uint32_t>(8u))
			return CM_LOW_COLOR;
		else
			return CM_MONOCHROME;
	}

	void UNIXTerminalBinding::registerSpecialKeys() {
#define reg(skey, ktype, kmod, index) registerSpecialKey(spec.skey, KeySym::ktype, KeySym::kmod, index);
#define chr(c) static_cast<unsigned>(static_cast<unsigned char>(c))
		reg(key_backspace, T_GENERIC, M_NONE, chr('\b'))
		reg(key_beg, T_BEGIN, M_NONE, 0u)
		reg(key_btab, T_BACK_TAB, M_NONE, 0u)
		reg(key_cancel, T_CANCEL, M_NONE, 0u)
		reg(key_catab, T_CLEAR_ALL_TABS, M_NONE, 0u)
		reg(key_clear, T_CLEAR_SCREEN, M_NONE, 0u)
		reg(key_close, T_CLOSE, M_NONE, 0u)
		reg(key_command, T_COMMAND, M_NONE, 0u)
		reg(key_copy, T_COPY, M_NONE, 0u)
		reg(key_create, T_CREATE, M_NONE, 0u)
		reg(key_ctab, T_CLEAR_TAB, M_NONE, 0u)
		reg(key_dc, T_DELETE, M_NONE, 0u)
		reg(key_dl, T_DELETE_LINE, M_NONE, 0u)
		reg(key_down, T_DOWN, M_NONE, 0u)
		reg(key_end, T_END, M_NONE, 0u)
		reg(key_enter, T_ENTER, M_NONE, 0u)
		reg(key_eol, T_CLEAR_TO_END_OF_LINE, M_NONE, 0u)
		reg(key_eos, T_CLEAR_TO_END_OF_SCREEN, M_NONE, 0u)
		reg(key_exit, T_EXIT, M_NONE, 0u)
		reg(key_f0, T_FUNCTION, M_NONE, 0u)
		reg(key_f1, T_FUNCTION, M_NONE, 1u)
		reg(key_f10, T_FUNCTION, M_NONE, 10u)
		reg(key_f11, T_FUNCTION, M_NONE, 11u)
		reg(key_f12, T_FUNCTION, M_NONE, 12u)
		reg(key_f13, T_FUNCTION, M_NONE, 13u)
		reg(key_f14, T_FUNCTION, M_NONE, 14u)
		reg(key_f15, T_FUNCTION, M_NONE, 15u)
		reg(key_f16, T_FUNCTION, M_NONE, 16u)
		reg(key_f17, T_FUNCTION, M_NONE, 17u)
		reg(key_f18, T_FUNCTION, M_NONE, 18u)
		reg(key_f19, T_FUNCTION, M_NONE, 19u)
		reg(key_f2, T_FUNCTION, M_NONE, 2u)
		reg(key_f20, T_FUNCTION, M_NONE, 20u)
		reg(key_f21, T_FUNCTION, M_NONE, 21u)
		reg(key_f22, T_FUNCTION, M_NONE, 22u)
		reg(key_f23, T_FUNCTION, M_NONE, 23u)
		reg(key_f24, T_FUNCTION, M_NONE, 24u)
		reg(key_f25, T_FUNCTION, M_NONE, 25u)
		reg(key_f26, T_FUNCTION, M_NONE, 26u)
		reg(key_f27, T_FUNCTION, M_NONE, 27u)
		reg(key_f28, T_FUNCTION, M_NONE, 28u)
		reg(key_f29, T_FUNCTION, M_NONE, 29u)
		reg(key_f3, T_FUNCTION, M_NONE, 3u)
		reg(key_f30, T_FUNCTION, M_NONE, 30u)
		reg(key_f31, T_FUNCTION, M_NONE, 31u)
		reg(key_f32, T_FUNCTION, M_NONE, 32u)
		reg(key_f33, T_FUNCTION, M_NONE, 33u)
		reg(key_f34, T_FUNCTION, M_NONE, 34u)
		reg(key_f35, T_FUNCTION, M_NONE, 35u)
		reg(key_f36, T_FUNCTION, M_NONE, 36u)
		reg(key_f37, T_FUNCTION, M_NONE, 37u)
		reg(key_f38, T_FUNCTION, M_NONE, 38u)
		reg(key_f39, T_FUNCTION, M_NONE, 39u)
		reg(key_f4, T_FUNCTION, M_NONE, 4u)
		reg(key_f40, T_FUNCTION, M_NONE, 40u)
		reg(key_f41, T_FUNCTION, M_NONE, 41u)
		reg(key_f42, T_FUNCTION, M_NONE, 42u)
		reg(key_f43, T_FUNCTION, M_NONE, 43u)
		reg(key_f44, T_FUNCTION, M_NONE, 44u)
		reg(key_f45, T_FUNCTION, M_NONE, 45u)
		reg(key_f46, T_FUNCTION, M_NONE, 46u)
		reg(key_f47, T_FUNCTION, M_NONE, 47u)
		reg(key_f48, T_FUNCTION, M_NONE, 48u)
		reg(key_f49, T_FUNCTION, M_NONE, 49u)
		reg(key_f5, T_FUNCTION, M_NONE, 5u)
		reg(key_f50, T_FUNCTION, M_NONE, 50u)
		reg(key_f51, T_FUNCTION, M_NONE, 51u)
		reg(key_f52, T_FUNCTION, M_NONE, 52u)
		reg(key_f53, T_FUNCTION, M_NONE, 53u)
		reg(key_f54, T_FUNCTION, M_NONE, 54u)
		reg(key_f55, T_FUNCTION, M_NONE, 55u)
		reg(key_f56, T_FUNCTION, M_NONE, 56u)
		reg(key_f57, T_FUNCTION, M_NONE, 57u)
		reg(key_f58, T_FUNCTION, M_NONE, 58u)
		reg(key_f59, T_FUNCTION, M_NONE, 59u)
		reg(key_f6, T_FUNCTION, M_NONE, 6u)
		reg(key_f60, T_FUNCTION, M_NONE, 60u)
		reg(key_f61, T_FUNCTION, M_NONE, 61u)
		reg(key_f62, T_FUNCTION, M_NONE, 62u)
		reg(key_f63, T_FUNCTION, M_NONE, 63u)
		reg(key_f7, T_FUNCTION, M_NONE, 7u)
		reg(key_f8, T_FUNCTION, M_NONE, 8u)
		reg(key_f9, T_FUNCTION, M_NONE, 9u)
		reg(key_find, T_FIND, M_NONE, 0u)
		reg(key_help, T_HELP, M_NONE, 0u)
		reg(key_home, T_HOME, M_NONE, 0u)
		reg(key_ic, T_INSERT, M_NONE, 0u)
		reg(key_il, T_INSERT_LINE, M_NONE, 0u)
		reg(key_left, T_LEFT, M_NONE, 0u)
		reg(key_ll, T_LOWER_LEFT, M_NONE, 0u)
		reg(key_mark, T_MARK, M_NONE, 0u)
		reg(key_message, T_MESSAGE, M_NONE, 0u)
		reg(key_move, T_MOVE, M_NONE, 0u)
		reg(key_next, T_NEXT, M_NONE, 0u)
		reg(key_npage, T_PAGE_DOWN, M_NONE, 0u)
		reg(key_open, T_OPEN, M_NONE, 0u)
		reg(key_options, T_OPTIONS, M_NONE, 0u)
		reg(key_ppage, T_PAGE_UP, M_NONE, 0u)
		reg(key_previous, T_PREVIOUS, M_NONE, 0u)
		reg(key_print, T_PRINT, M_NONE, 0u)
		reg(key_redo, T_REDO, M_NONE, 0u)
		reg(key_reference, T_REFERENCE, M_NONE, 0u)
		reg(key_refresh, T_REFRESH, M_NONE, 0u)
		reg(key_replace, T_REPLACE, M_NONE, 0u)
		reg(key_restart, T_RESTART, M_NONE, 0u)
		reg(key_resume, T_RESUME, M_NONE, 0u)
		reg(key_right, T_RIGHT, M_NONE, 0u)
		reg(key_save, T_SAVE, M_NONE, 0u)
		reg(key_sbeg, T_BEGIN, M_SHIFT, 0u)
		reg(key_scancel, T_CANCEL, M_SHIFT, 0u)
		reg(key_scommand, T_COMMAND, M_SHIFT, 0u)
		reg(key_scopy, T_COPY, M_SHIFT, 0u)
		reg(key_screate, T_CREATE, M_SHIFT, 0u)
		reg(key_sdc, T_DELETE, M_SHIFT, 0u)
		reg(key_sdl, T_DELETE_LINE, M_SHIFT, 0u)
		reg(key_select, T_SELECT, M_NONE, 0u)
		reg(key_send, T_END, M_SHIFT, 0u)
		reg(key_seol, T_CLEAR_TO_END_OF_LINE, M_SHIFT, 0u)
		reg(key_sexit, T_EXIT, M_SHIFT, 0u)
		//reg(key_sf, T_SCROLL_FORWARD, M_NONE, 0u) <-- used in place of missing key_sdown
		reg(key_sf, T_DOWN, M_SHIFT, 0u)
		reg(key_sfind, T_FIND, M_SHIFT, 0u)
		reg(key_shelp, T_HELP, M_SHIFT, 0u)
		reg(key_shome, T_HOME, M_SHIFT, 0u)
		reg(key_sic, T_INSERT, M_SHIFT, 0u)
		reg(key_sleft, T_LEFT, M_SHIFT, 0u)
		reg(key_smessage, T_MESSAGE, M_SHIFT, 0u)
		reg(key_smove, T_MOVE, M_SHIFT, 0u)
		reg(key_snext, T_NEXT, M_SHIFT, 0u)
		reg(key_soptions, T_OPTIONS, M_SHIFT, 0u)
		reg(key_sprevious, T_PREVIOUS, M_SHIFT, 0u)
		reg(key_sprint, T_PRINT, M_SHIFT, 0u)
		//reg(key_sr, T_SCROLL_BACKWARD, M_NONE, 0u) <-- used in place of missing key_sup
		reg(key_sr, T_UP, M_SHIFT, 0u)
		reg(key_sredo, T_REDO, M_SHIFT, 0u)
		reg(key_sreplace, T_REPLACE, M_SHIFT, 0u)
		reg(key_sright, T_RIGHT, M_SHIFT, 0u)
		reg(key_srsume, T_RESUME, M_SHIFT, 0u)
		reg(key_ssave, T_SAVE, M_SHIFT, 0u)
		reg(key_ssuspend, T_SUSPEND, M_SHIFT, 0u)
		reg(key_stab, T_SET_TAB, M_NONE, 0u)
		reg(key_sundo, T_UNDO, M_SHIFT, 0u)
		reg(key_suspend, T_SUSPEND, M_NONE, 0u)
		reg(key_undo, T_UNDO, M_NONE, 0u)
		reg(key_up, T_UP, M_NONE, 0u)
		// hacks
		if(spec.key_left && !strcmp(spec.key_left, "\033OD") && !hasSpecialKey("\033[D"))
			registerSpecialKey("\033[D", KeySym::T_LEFT, KeySym::M_NONE, 0u);
		if(spec.key_right && !strcmp(spec.key_right, "\033OC") && !hasSpecialKey("\033[C"))
			registerSpecialKey("\033[C", KeySym::T_RIGHT, KeySym::M_NONE, 0u);
		if(spec.key_up && !strcmp(spec.key_up, "\033OA") && !hasSpecialKey("\033[A"))
			registerSpecialKey("\033[A", KeySym::T_UP, KeySym::M_NONE, 0u);
		if(spec.key_down && !strcmp(spec.key_down, "\033OB") && !hasSpecialKey("\033[B"))
			registerSpecialKey("\033[B", KeySym::T_DOWN, KeySym::M_NONE, 0u);
		if(spec.key_home && !strcmp(spec.key_home, "\033OH") && !hasSpecialKey("\033[H"))
			registerSpecialKey("\033[H", KeySym::T_HOME, KeySym::M_NONE, 0u);
		if(spec.key_end && !strcmp(spec.key_end, "\033OF") && !hasSpecialKey("\033[F"))
			registerSpecialKey("\033[F", KeySym::T_END, KeySym::M_NONE, 0u);
#undef chr
#undef reg
	}

	void UNIXTerminalBinding::registerSpecialKey(const char* controlSequence,
			KeySym::Type type, KeySym::Modifier modifier, unsigned index) {
		if(!controlSequence || !*controlSequence)
			return;
		SequenceMapInputConverter::Map::Node* node = &specialKeyMap.getRoot();
		for(; *controlSequence; ++controlSequence)
			node = &node->put(*controlSequence);
		node->setValue(KeySym(type, modifier, static_cast<Char16>(index)));
	}

	bool UNIXTerminalBinding::hasSpecialKey(const char* controlSequence) const {
		const SequenceMapInputConverter::Map::Node* node = &specialKeyMap.getRoot();
		for(; *controlSequence; ++controlSequence) {
			const SequenceMapInputConverter::Map::Node* nextNode = node->get(*controlSequence);
			if(!nextNode)
				return false;
			node = nextNode;
		}
		return node->hasValue();
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

#define GET_CODEC_MANAGER (codecManager ? *codecManager : CodecManager::getDefaultCodecManager())

	void UNIXTerminalBinding::setEncoderFactoryFromEncoding() {
		Unref<CodecFactory<Encoder16> > newFactory;
		if(encname.empty())
			newFactory = new DefaultCodecFactory<Encoder16, UTF8Encoder16>;
		else {
			newFactory = GET_CODEC_MANAGER.loadEncoder16Factory(encname);
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
		if(encname.empty()) {
			newFactory = new DefaultCodecFactory<Decoder16, UTF8Decoder16>;
			usingUTF8 = true;
		}
		else {
			newFactory = GET_CODEC_MANAGER.loadDecoder16Factory(encname);
			if(*newFactory) {
				newFactory->ref();
				usingUTF8 = encname == "UTF-8" || encname == "utf8";
			}
			else {
				newFactory = new DefaultCodecFactory<Decoder16, UTF8Decoder16>;
				usingUTF8 = true;
			}
		}
		if(decoderFactory)
			decoderFactory->unref();
		decoderFactory = *newFactory;
		decoderFactory->ref();
	}

	void UNIXTerminalBinding::dumpCapabilities() const {
		cerr << "Present terminal capabilities:" << endl;
#define capp(name) cerr << "    - " #name ": " << (spec.name ? "yes" : "no") << endl;
		capp(carriage_return)
		capp(clear_screen)
		capp(cursor_address)
		capp(row_address)
		capp(column_address)
		capp(cursor_left)
		capp(cursor_right)
		capp(cursor_up)
		capp(cursor_down)
		capp(parm_left_cursor)
		capp(parm_right_cursor)
		capp(parm_up_cursor)
		capp(parm_down_cursor)
		capp(set_attributes)
#undef capp
		cerr << "Max " << spec.max_colors << " colors -> ";
		switch(colorMode) {
#define clamp(cons) \
	case cons: \
		cerr << #cons << endl; \
		break;
			clamp(CM_MONOCHROME)
			clamp(CM_LOW_COLOR)
			clamp(CM_HIGH_COLOR)
#undef clamp
			default:
				cerr << "<unknown>" << endl;
				break;
		}
	}

	void UNIXTerminalBinding::setWriteMode(WriteMode mode) {
		if(mode == writeMode)
			return;
		flush();
		char* buffer = mode == WM_BLOCKED ? new char[REDSTRAIN_DAMNATION_UNIXTERMINALBINDING_BLOCK_SIZE] : NULL;
		switch(writeMode) {
			case WM_DIRECT:
				break;
			case WM_BLOCKED:
				delete[] writeBlock;
				writeBlock = NULL;
				break;
			case WM_BUFFERED:
				// The call to flush() above should already clear writeBuffer for us,
				// but since some nitwit might override it, we'll do it just to be sure.
				writeBuffer.clear();
				break;
			default:
				throw ProgrammingError("Unrecognized WriteMode in UNIXTerminalBinding::setWriteMode(): "
						+ StringUtils::toString(static_cast<int>(writeMode)));
		}
		switch(mode) {
			case WM_DIRECT:
			case WM_BUFFERED:
				break;
			case WM_BLOCKED:
				writeBlock = buffer;
				break;
			default:
				throw IllegalUNIXTerminalWriteModeError(mode);
		}
		writeMode = mode;
	}

	void UNIXTerminalBinding::wrappedWrite(const char* data, MemorySize size) {
		switch(writeMode) {
			case WM_DIRECT:
				doWrite(data, size);
				break;
			case WM_BLOCKED:
				{
					MemorySize available
							= static_cast<MemorySize>(REDSTRAIN_DAMNATION_UNIXTERMINALBINDING_BLOCK_SIZE) - blockFill;
					while(size) {
						MemorySize chunk = size < available ? size : available;
						memcpy(writeBlock + blockFill, data, static_cast<size_t>(chunk));
						data += chunk;
						blockFill += chunk;
						available -= chunk;
						if(!available) {
							available = blockFill;
							blockFill = static_cast<MemorySize>(0u);
							doWrite(writeBlock, available);
						}
					}
				}
				break;
			case WM_BUFFERED:
				writeBuffer.append(data, size);
				break;
			default:
				throw ProgrammingError("Unrecognized WriteMode in UNIXTerminalBinding::wrappedWrite(): "
						+ StringUtils::toString(static_cast<int>(writeMode)));
		}
	}

	void UNIXTerminalBinding::wrappedWrite(const string& data) {
		wrappedWrite(data.data(), static_cast<MemorySize>(data.length()));
	}

#if defined(REDSTRAIN_ENV_OS_LINUX)

	void UNIXTerminalBinding::startTerminalUse(bool saveScreenContents) {
		// So yeah, technically, we should use smcup if and only if we
		// use cup, but the fact of the matter is that modern terminals
		// seem to interpret it as "switch to alternate screen" instead,
		// with no actual impact on cup usability...
		if(saveScreenContents && !inCAMode && spec.enter_ca_mode && spec.exit_ca_mode) {
			wrappedWrite(termParam(spec.enter_ca_mode));
			inCAMode = true;
		}
	}

	void UNIXTerminalBinding::stopTerminalUse() {
		if(inCAMode) {
			wrappedWrite(termParam(spec.exit_ca_mode));
			inCAMode = false;
		}
	}

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
		bool doUTF8 = usingUTF8 || (currentState.c_iflag & IUTF8);
#define set(type, flags) (currentState.c_ ## type ## flag |= (flags))
#define clear(type, flags) (currentState.c_ ## type ## flag &= ~(flags))
		set(i, IGNBRK);
		clear(i, PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IUCLC | IXON | IXOFF);
		if(doUTF8)
			set(i, IUTF8);
		else
			clear(i, IUTF8);
		set(o, OPOST);
		clear(o, OLCUC);
		set(l, IEXTEN);
		clear(l, ISIG | ICANON | ECHO);
#undef set
#undef clear
		currentState.c_cc[VTIME] = static_cast<cc_t>(0);
		currentState.c_cc[VMIN] = static_cast<cc_t>(!!currentlyBlocking);
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

	static void getTermSize(int fd, unsigned& width, unsigned& height) {
		struct winsize wsz;
		if(ioctl(fd, TIOCGWINSZ, &wsz))
			width = height = 0u;
		else {
			width = static_cast<unsigned>(wsz.ws_col);
			height = static_cast<unsigned>(wsz.ws_row);
		}
	}

	bool UNIXTerminalBinding::hasSizeChanged() {
		unsigned width, height;
		getTermSize(writefd, width, height);
		return width != prevWidth || height != prevHeight;
	}

	Size UNIXTerminalBinding::getSize() {
		getTermSize(writefd, prevWidth, prevHeight);
		return Size(prevWidth, prevHeight);
	}

	bool UNIXTerminalBinding::supportsOperation(OptionalOperation operation) {
		switch(operation) {
			case OO_CARRIAGE_RETURN:
				return !!spec.carriage_return;
			case OO_CLEAR_SCREEN:
				return !!spec.clear_screen;
			case OO_MOVE_TO:
				if(spec.cursor_address)
					return true;
				if(!spec.row_address)
					return false;
				if(spec.column_address)
					return true;
				return spec.carriage_return && (spec.parm_right_cursor || spec.cursor_right);
			case OO_ROW_TO:
				return !!spec.row_address;
			case OO_COLUMN_TO:
				if(spec.column_address)
					return true;
				return spec.carriage_return && (spec.parm_right_cursor || spec.cursor_right);
			case OO_MOVE_BY:
				if(!spec.parm_left_cursor && !spec.cursor_left)
					return false;
				if(!spec.parm_right_cursor && !spec.cursor_right)
					return false;
				if(!spec.parm_up_cursor && !spec.cursor_up)
					return false;
				if(!spec.parm_down_cursor && !spec.cursor_down && !spec.cursor_down_or_scroll)
					return false;
				return true;
			case OO_ROW_BY:
				if(!spec.parm_up_cursor && !spec.cursor_up)
					return false;
				if(!spec.parm_down_cursor && !spec.cursor_down && !spec.cursor_down_or_scroll)
					return false;
				return true;
			case OO_COLUMN_BY:
				if(!spec.parm_left_cursor && !spec.cursor_left)
					return false;
				if(!spec.parm_right_cursor && !spec.cursor_right)
					return false;
				return true;
			case OO_CURSOR_DOWN:
			case OO_CURSOR_DOWN_BY:
				return spec.parm_down_cursor || spec.cursor_down || spec.cursor_down_or_scroll;
			case OO_CURSOR_UP:
			case OO_CURSOR_UP_BY:
				return spec.parm_up_cursor || spec.cursor_up;
			case OO_CURSOR_LEFT:
			case OO_CURSOR_LEFT_BY:
				return spec.parm_left_cursor || spec.cursor_left;
			case OO_CURSOR_RIGHT:
			case OO_CURSOR_RIGHT_BY:
				return spec.parm_right_cursor || spec.cursor_right;
			default:
				return false;
		}
	}

	bool UNIXTerminalBinding::hasAutoRightMargin() {
		return spec.hasFlag(TSF_AUTO_RIGHT_MARGIN);
	}

	unsigned UNIXTerminalBinding::getTabSpacing() {
		unsigned tabs = static_cast<unsigned>(spec.init_tabs);
		return tabs ? tabs : 1u;
	}

	bool UNIXTerminalBinding::carriageReturn() {
		if(!spec.carriage_return)
			return false;
		wrappedWrite(termParam(spec.carriage_return));
		return true;
	}

	void UNIXTerminalBinding::newLine() {
		if(spec.carriage_return && spec.newline) {
			wrappedWrite(termParam(spec.carriage_return));
			wrappedWrite(termParam(spec.newline));
		}
		else
			wrappedWrite("\r\n");  // Let's call that a "good default"...
	}

	bool UNIXTerminalBinding::clearScreen() {
		if(!spec.clear_screen)
			return false;
		wrappedWrite(termParam(spec.clear_screen));
		return true;
	}

	bool UNIXTerminalBinding::moveTo(unsigned row, unsigned column) {
		if(spec.cursor_address) {
			wrappedWrite(termParam(spec.cursor_address, static_cast<int32_t>(row), static_cast<int32_t>(column)));
			return true;
		}
		if(!spec.row_address)
			return false;
		if(spec.column_address) {
			wrappedWrite(termParam(spec.row_address, static_cast<int32_t>(row)));
			wrappedWrite(termParam(spec.column_address, static_cast<int32_t>(column)));
			return true;
		}
		if(!spec.carriage_return)
			return false;
		if(spec.parm_right_cursor) {
			wrappedWrite(termParam(spec.carriage_return));
			wrappedWrite(termParam(spec.row_address, static_cast<int32_t>(row)));
			wrappedWrite(termParam(spec.parm_right_cursor, static_cast<int32_t>(column)));
			return true;
		}
		if(!spec.cursor_right)
			return false;
		wrappedWrite(termParam(spec.carriage_return));
		wrappedWrite(termParam(spec.row_address, static_cast<int32_t>(row)));
		string right(termParam(spec.cursor_right));
		for(; column; --column)
			wrappedWrite(right);
		return true;
	}

	bool UNIXTerminalBinding::rowTo(unsigned row) {
		if(!spec.row_address)
			return false;
		wrappedWrite(termParam(spec.row_address, static_cast<int32_t>(row)));
		return true;
	}

	bool UNIXTerminalBinding::columnTo(unsigned column) {
		if(spec.column_address) {
			wrappedWrite(termParam(spec.column_address, static_cast<int32_t>(column)));
			return true;
		}
		if(!spec.carriage_return)
			return false;
		if(spec.parm_right_cursor) {
			wrappedWrite(termParam(spec.carriage_return));
			wrappedWrite(termParam(spec.parm_right_cursor, static_cast<int32_t>(column)));
			return true;
		}
		wrappedWrite(termParam(spec.carriage_return));
		string right(termParam(spec.cursor_right));
		for(; column; --column)
			wrappedWrite(right);
		return true;
	}

	bool UNIXTerminalBinding::moveBy(int rows, int columns) {
		if(rows < 0) {
			if(!spec.parm_up_cursor && !spec.cursor_up)
				return false;
		}
		else if(rows > 0) {
			if(!spec.parm_down_cursor && !spec.cursor_down && !spec.cursor_down_or_scroll)
				return false;
		}
		if(columns < 0) {
			if(!spec.parm_left_cursor && !spec.cursor_left)
				return false;
		}
		else if(columns > 0) {
			if(!spec.parm_right_cursor && !spec.cursor_right)
				return false;
		}
		if(rows > 0) {
			if(spec.parm_down_cursor)
				wrappedWrite(termParam(spec.parm_down_cursor, static_cast<int32_t>(rows)));
			else {
				string down(termParam(spec.cursor_down ? spec.cursor_down : spec.cursor_down_or_scroll));
				for(; rows; --rows)
					wrappedWrite(down);
			}
		}
		else if(rows < 0) {
			if(spec.parm_up_cursor)
				wrappedWrite(termParam(spec.parm_up_cursor, static_cast<int32_t>(-rows)));
			else {
				string up(termParam(spec.cursor_up));
				for(; rows; ++rows)
					wrappedWrite(up);
			}
		}
		if(columns > 0) {
			if(spec.parm_right_cursor)
				wrappedWrite(termParam(spec.parm_right_cursor, static_cast<int32_t>(columns)));
			else {
				string right(termParam(spec.cursor_right));
				for(; columns; --columns)
					wrappedWrite(right);
			}
		}
		else if(columns < 0) {
			if(spec.parm_left_cursor)
				wrappedWrite(termParam(spec.parm_left_cursor, static_cast<int32_t>(-columns)));
			else {
				string left(termParam(spec.cursor_left));
				for(; columns; ++columns)
					wrappedWrite(left);
			}
		}
		return true;
	}

	bool UNIXTerminalBinding::rowBy(int rows) {
		if(rows < 0) {
			if(spec.parm_up_cursor) {
				wrappedWrite(termParam(spec.parm_up_cursor, static_cast<int32_t>(-rows)));
				return true;
			}
			else if(spec.cursor_up) {
				string up(termParam(spec.cursor_up));
				for(; rows; ++rows)
					wrappedWrite(up);
				return true;
			}
			else
				return false;
		}
		else if(rows > 0) {
			if(spec.parm_down_cursor) {
				wrappedWrite(termParam(spec.parm_down_cursor, static_cast<int32_t>(rows)));
				return true;
			}
			else if(spec.cursor_down || spec.cursor_down_or_scroll) {
				string down(termParam(spec.cursor_down ? spec.cursor_down : spec.cursor_down_or_scroll));
				for(; rows; --rows)
					wrappedWrite(down);
				return true;
			}
			else
				return false;
		}
		else
			return true;
	}

	bool UNIXTerminalBinding::columnBy(int columns) {
		if(columns < 0) {
			if(spec.parm_left_cursor) {
				wrappedWrite(termParam(spec.parm_left_cursor, static_cast<int32_t>(-columns)));
				return true;
			}
			else if(spec.cursor_left) {
				string left(termParam(spec.cursor_left));
				for(; columns; ++columns)
					wrappedWrite(left);
				return true;
			}
			else
				return false;
		}
		else if(columns > 0) {
			if(spec.parm_right_cursor) {
				wrappedWrite(termParam(spec.parm_right_cursor, static_cast<int32_t>(columns)));
				return true;
			}
			else if(spec.cursor_right) {
				string right(termParam(spec.cursor_right));
				for(; columns; --columns)
					wrappedWrite(right);
				return true;
			}
			else
				return false;
		}
		else
			return true;
	}

	bool UNIXTerminalBinding::cursorDown() {
		if(spec.cursor_down || spec.cursor_down_or_scroll) {
			wrappedWrite(termParam(spec.cursor_down ? spec.cursor_down : spec.cursor_down_or_scroll));
			return true;
		}
		if(spec.parm_down_cursor) {
			wrappedWrite(termParam(spec.parm_down_cursor, static_cast<int32_t>(1)));
			return true;
		}
		return false;
	}

	bool UNIXTerminalBinding::cursorDownBy(unsigned rows) {
		if(!rows)
			return true;
		if(spec.parm_down_cursor) {
			wrappedWrite(termParam(spec.parm_down_cursor, static_cast<int32_t>(rows)));
			return true;
		}
		if(spec.cursor_down || spec.cursor_down_or_scroll) {
			string down(termParam(spec.cursor_down ? spec.cursor_down : spec.cursor_down_or_scroll));
			for(; rows; --rows)
				wrappedWrite(down);
			return true;
		}
		return false;
	}

	bool UNIXTerminalBinding::cursorUp() {
		if(spec.cursor_up) {
			wrappedWrite(termParam(spec.cursor_up));
			return true;
		}
		if(spec.parm_up_cursor) {
			wrappedWrite(termParam(spec.parm_up_cursor, static_cast<int32_t>(1)));
			return true;
		}
		return false;
	}

	bool UNIXTerminalBinding::cursorUpBy(unsigned rows) {
		if(!rows)
			return true;
		if(spec.parm_up_cursor) {
			wrappedWrite(termParam(spec.parm_up_cursor, static_cast<int32_t>(rows)));
			return true;
		}
		if(spec.cursor_up) {
			string up(termParam(spec.cursor_up));
			for(; rows; --rows)
				wrappedWrite(up);
			return true;
		}
		return false;
	}

	bool UNIXTerminalBinding::cursorLeft() {
		if(spec.cursor_left) {
			wrappedWrite(termParam(spec.cursor_left));
			return true;
		}
		if(spec.parm_left_cursor) {
			wrappedWrite(termParam(spec.parm_left_cursor, static_cast<int32_t>(1)));
			return true;
		}
		return false;
	}

	bool UNIXTerminalBinding::cursorLeftBy(unsigned columns) {
		if(!columns)
			return true;
		if(spec.parm_left_cursor) {
			wrappedWrite(termParam(spec.parm_left_cursor, static_cast<int32_t>(columns)));
			return true;
		}
		if(spec.cursor_left) {
			string left(termParam(spec.cursor_left));
			for(; columns; --columns)
				wrappedWrite(left);
			return true;
		}
		return false;
	}

	bool UNIXTerminalBinding::cursorRight() {
		if(spec.cursor_right) {
			wrappedWrite(termParam(spec.cursor_right));
			return true;
		}
		if(spec.parm_right_cursor) {
			wrappedWrite(termParam(spec.parm_right_cursor, static_cast<int32_t>(1)));
			return true;
		}
		return false;
	}

	bool UNIXTerminalBinding::cursorRightBy(unsigned columns) {
		if(!columns)
			return true;
		if(spec.parm_right_cursor) {
			wrappedWrite(termParam(spec.parm_right_cursor, static_cast<int32_t>(columns)));
			return true;
		}
		if(spec.cursor_right) {
			string right(termParam(spec.cursor_right));
			for(; columns; --columns)
				wrappedWrite(right);
			return true;
		}
		return false;
	}

	unsigned UNIXTerminalBinding::getColorCount() {
		return spec.max_colors < static_cast<uint32_t>(2u) ? 2u : static_cast<unsigned>(spec.max_colors);
	}

	void UNIXTerminalBinding::doWrite(const char* buffer, MemorySize size) {
		while(size) {
			ssize_t count = ::write(writefd, buffer, static_cast<size_t>(size));
			if(count == static_cast<ssize_t>(-1))
				throw FileIOError(File::OUTPUT, errno);
			buffer += count;
			size -= static_cast<MemorySize>(count);
		}
	}

#define GET_COLOR_MAP (colorMap ? *colorMap : ColorMap::getDefaultColorMap())

	unsigned UNIXTerminalBinding::setColor(unsigned base, unsigned color) {
		if(color >= 256u)
			throw IllegalColorIndexError(color);
		switch(colorMode) {
			case CM_MONOCHROME:
				return base == 30u ? 7u : 0u;
			case CM_LOW_COLOR:
				{
					color = GET_COLOR_MAP.highToLowColor(color);
					stringstream ss;
					ss << "\033[" << (base + color) << 'm';
					wrappedWrite(ss.str());
					return color;
				}
			case CM_HIGH_COLOR:
				{
					stringstream ss;
					ss << "\033[" << (base + 8u) << ";5;" << color << 'm';
					wrappedWrite(ss.str());
					return color;
				}
			default:
				throw ProgrammingError("Unrecognized ColorMode in UNIXTerminalBinding::setColor(): "
						+ StringUtils::toString(static_cast<int>(colorMode)));
		}
	}

	unsigned UNIXTerminalBinding::setForegroundColor(unsigned color) {
		return setColor(30u, color);
	}

	unsigned UNIXTerminalBinding::setBackgroundColor(unsigned color) {
		return setColor(40u, color);
	}

#define addAttr(cons) currentAttributes |= UNIXTerminalBinding::cons
#define delAttr(cons) currentAttributes &= ~UNIXTerminalBinding::cons

	void UNIXTerminalBinding::enterUnderline() {
		if(spec.enter_underline_mode)
			wrappedWrite(termParam(spec.enter_underline_mode));
		else
			wrappedWrite("\033[4m");
		addAttr(TA_UNDERLINE);
	}

	void UNIXTerminalBinding::leaveUnderline() {
		if(spec.exit_underline_mode)
			wrappedWrite(termParam(spec.exit_underline_mode));
		else
			wrappedWrite("\033[24m");
		delAttr(TA_UNDERLINE);
	}

#undef addAttr
#undef delAttr

#define forAttr(cons) \
	have = !!(currentAttributes & UNIXTerminalBinding::cons); \
	want = !!(attributes & UNIXTerminalBinding::cons)

	int UNIXTerminalBinding::setTextAttributes(int attributes) {
		bool have, want;
		forAttr(TA_UNDERLINE);
		if(want && !have)
			enterUnderline();
		else if(have && !want)
			leaveUnderline();
		return currentAttributes;
	}

	int UNIXTerminalBinding::addTextAttributes(int attributes) {
		bool have, want;
		forAttr(TA_UNDERLINE);
		if(want && !have)
			enterUnderline();
		return currentAttributes;
	}

	int UNIXTerminalBinding::removeTextAttributes(int attributes) {
		bool have, want;
		forAttr(TA_UNDERLINE);
		if(have && !want)
			leaveUnderline();
		return currentAttributes;
	}

#undef forAttr

	void UNIXTerminalBinding::resetTextAttributes() {
		if(spec.exit_underline_mode && (currentAttributes & UNIXTerminalBinding::TA_UNDERLINE))
			wrappedWrite(spec.exit_underline_mode);
		wrappedWrite("\033[0m");
		currentAttributes = 0;
	}

	static bool setBlocking(int readfd, struct termios& currentState, bool& currentlyBlocking, bool blocking) {
		if(!(currentState.c_lflag & ICANON) && !blocking != !currentlyBlocking) {
			currentState.c_cc[VTIME] = static_cast<cc_t>(0);
			currentState.c_cc[VMIN] = static_cast<cc_t>(!!blocking);
			if(!setattr(readfd, TCSADRAIN, currentState))
				return false;
			currentlyBlocking = blocking;
		}
		return true;
	}

	static MemorySize doRead(int readfd, char* buffer, size_t size, bool& winch) {
		for(;;) {
			ssize_t count = ::read(readfd, buffer, size);
			if(count != static_cast<ssize_t>(-1)) {
				winch = false;
				return static_cast<MemorySize>(count);
			}
			if(errno != EINTR)
				throw FileIOError(File::INPUT, errno);
			if(caughtWinch) {
				caughtWinch = false;
				winch = true;
				return static_cast<MemorySize>(0u);
			}
		}
	}

	static void doSleep(uint64_t delay) {
		uint64_t start = currentTimeMillis();
		for(;;) {
			Thread::sleep(delay);
			uint64_t now = currentTimeMillis();
			uint64_t delta = now - start;
			if(delta >= delay)
				break;
			delay -= delta;
			start = now;
		}
	}

	KeySym UNIXTerminalBinding::read(bool blocking) {
		if(caughtWinch) {
			caughtWinch = false;
			return KeySym::WINCH;
		}
		KeySym key(inputConverter.nextKey());
		if(key.getType() != KeySym::T_NONE)
			return key;
		// prepare for read(2)
		struct termios currentState;
		if(!getattr(readfd, currentState))
			throw FileIOError(File::INPUT, errno);
		if(!setBlocking(readfd, currentState, currentlyBlocking, blocking))
			throw FileIOError(File::INPUT, errno);
		// do the actual read(2)
		char buffer[128];
		bool winch;
		MemorySize count = doRead(readfd, buffer, sizeof(buffer), winch);
		if(winch)
			return KeySym::WINCH;
		if(!count)
			return KeySym::NONE;
		inputConverter.feedInput(buffer, count);
		if(!setBlocking(readfd, currentState, currentlyBlocking, false))
			throw FileIOError(File::INPUT, errno);
		for(;;) {
			count = doRead(readfd, buffer, sizeof(buffer), winch);
			if(winch)
				return KeySym::WINCH;
			if(!count) {
				if(!inputConverter.hasPendingKey())
					break;
				doSleep(specialKeyFlushDelay);
				count = doRead(readfd, buffer, sizeof(buffer), winch);
				if(winch)
					return KeySym::WINCH;
				if(!count)
					break;
			}
			inputConverter.feedInput(buffer, count);
		}
		inputConverter.flushSequences();
		return inputConverter.nextKey();
	}

	void UNIXTerminalBinding::write(char c) {
		wrappedWrite(&c, static_cast<MemorySize>(1u));
	}

	void UNIXTerminalBinding::write(const string& data) {
		wrappedWrite(data);
	}

	void UNIXTerminalBinding::write(Char16 c) {
		write(String16(&c, static_cast<String16::size_type>(1u)));
	}

	void UNIXTerminalBinding::write(const String16& data) {
		Delete<Encoder16> encoder(encoderFactory->newCodec());
		wrappedWrite(Transcode::transcodeShortString2<Char16, char>(data, **encoder));
	}

	void UNIXTerminalBinding::write(BoxSymbol symbol) {
		try {
			write(getBoxChar16(symbol));
		}
		catch(const UnrepresentableCharacterError&) {
			write(getBoxChar8(symbol));
		}
	}

	void UNIXTerminalBinding::flush() {
		switch(writeMode) {
			case WM_DIRECT:
				break;
			case WM_BLOCKED:
				if(blockFill) {
					doWrite(writeBlock, blockFill);
					blockFill = static_cast<MemorySize>(0u);
				}
				break;
			case WM_BUFFERED:
				{
					MemorySize size = writeBuffer.getLength();
					if(size) {
						DeleteArray<char> buffer(writeBuffer.toCharArray());
						writeBuffer.clear();
						doWrite(*buffer, size);
					}
				}
				break;
			default:
				throw ProgrammingError("Unrecognized WriteMode in UNIXTerminalBinding::wrappedWrite(): "
						+ StringUtils::toString(static_cast<int>(writeMode)));
		}
	}

#else /* non-UNIX */

	void UNIXTerminalBinding::catchSIGWINCH() {}
	void UNIXTerminalBinding::setRaw() {}
	void UNIXTerminalBinding::setCooked() {}
	void UNIXTerminalBinding::enterUnderline() {}
	void UNIXTerminalBinding::leaveUnderline() {}

	void UNIXTerminalBinding::startTerminalUse(bool) {
		NOT_UNIX
	}

	void UNIXTerminalBinding::stopTerminalUse() {
		NOT_UNIX
	}

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

	void UNIXTerminalBinding::newLine() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::clearScreen() {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::moveTo(unsigned, unsigned) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::rowTo(unsigned) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::columnTo(unsigned) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::moveBy(int, int) {
		NOT_UNIX
	}

	bool UNIXTerminalBinding::rowBy(int) {
		NOT_UNIX;
	}

	bool UNIXTerminalBinding::columnBy(int) {
		NOT_UNIX;
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

	unsigned UNIXTerminalBinding::setColor(unsigned, unsigned) {
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

	void UNIXTerminalBinding::resetTextAttributes() {
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

	void UNIXTerminalBinding::doWrite(const char*, MemorySize) {
		NOT_UNIX
	}

#endif /* UNIX? */

}}
