#include <cstring>
#include <sstream>
#include <iostream>
#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/text/UTF8Encoder.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/util/DeleteArray.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/text/DefaultCodecFactory.hpp>
#include <redstrain/text/UnrepresentableCharacterError.hpp>
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

#include "ColorMap.hpp"
#include "UNIXTerminalBinding.hpp"
#include "IllegalColorIndexError.hpp"
#include "IllegalUNIXTerminalWriteModeError.hpp"
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
using redengine::text::Encoder16;
using redengine::text::Decoder16;
using redengine::text::Transcode;
using redengine::util::MemorySize;
using redengine::util::DeleteArray;
using redengine::util::StringUtils;
using redengine::text::CodecManager;
using redengine::text::CodecFactory;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF8Decoder16;
using redengine::error::ProgrammingError;
using redengine::text::DefaultCodecFactory;
using redengine::text::UnrepresentableCharacterError;
using redengine::error::UnsupportedPlatformFeatureError;
using redengine::text::guessTerminalCharset;

#define NOT_UNIX throw UnsupportedPlatformFeatureError("UNIXTerminalBinding");

namespace redengine {
namespace damnation {

	UNIXTerminalBinding::UNIXTerminalBinding(const TermSpec& spec) : readfd(0), writefd(1), spec(spec),
			codecManager(NULL), encoderFactory(NULL), decoderFactory(NULL), oldTermios(NULL), usingUTF8(false),
			prevWidth(0u), prevHeight(0u), colorMode(UNIXTerminalBinding::colorModeFromColorCount(spec.max_colors)),
			currentAttributes(0), writeMode(WM_DIRECT), writeBlock(NULL), blockFill(static_cast<MemorySize>(0u)) {
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
			decoderFactory(binding.decoderFactory), oldTermios(NULL), usingUTF8(binding.usingUTF8),
			prevWidth(binding.prevWidth), prevHeight(binding.prevHeight), colorMode(binding.colorMode),
			currentAttributes(binding.currentAttributes), writeMode(binding.writeMode),
			writeBlock(writeMode == WM_BLOCKED ? new char[REDSTRAIN_DAMNATION_UNIXTERMINALBINDING_BLOCK_SIZE]
			: NULL), blockFill(static_cast<MemorySize>(0u)) {
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

	KeySym UNIXTerminalBinding::read(bool) {
		//TODO
		return KeySym::NONE;
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

#endif /* UNIX? */

}}
