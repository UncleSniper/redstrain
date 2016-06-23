#include "Console.hpp"
#include "FileIOError.hpp"
#include "ConsoleError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <cstdio>
#include <cstring>
#include <sys/ioctl.h>
using redengine::util::MemorySize;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <redstrain/util/IntegerBounds.hpp>
using redengine::util::IntegerBounds;
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

	Console::Console(const File& file) : file(file) {}

	Console::Console(StandardHandle handle) : file(getStandardHandle(handle), File::OUTPUT) {}

	Console::Console(const Console& console) : file(console.file) {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

#define wrapWriteError(oper) \
	catch(const FileIOError& error) { \
		throw ConsoleError(error.getErrorCode(), ConsoleError::oper); \
	}

	void Console::clear() {
		try {
			file.write("\033[2J\033[H", static_cast<MemorySize>(7u));
		}
		wrapWriteError(CLEAR)
	}

	void Console::home() {
		try {
			file.write("\033[H", static_cast<MemorySize>(3u));
		}
		wrapWriteError(SET_CURSOR_POSITION)
	}

	static const char *const COLOR_DIGITS = "01234567";

	void Console::setForeground(Color color) {
		if(color < 0 || color > 7)
			return;
		char seq[6] = "\033[3?m";
		seq[3] = COLOR_DIGITS[color];
		try {
			file.write(seq, static_cast<MemorySize>(5u));
		}
		wrapWriteError(SET_TEXT_ATTRIBUTE)
	}

	void Console::setBackground(Color color) {
		if(color < 0 || color > 7)
			return;
		char seq[6] = "\033[4?m";
		seq[3] = COLOR_DIGITS[color];
		try {
			file.write(seq, static_cast<MemorySize>(5u));
		}
		wrapWriteError(SET_TEXT_ATTRIBUTE)
	}

	void Console::setIntensity(Intensity intensity) {
		try {
			if(intensity == NORMAL)
				file.write("\033[22m", static_cast<MemorySize>(5u));
			else
				file.write("\033[1m", static_cast<MemorySize>(4u));
		}
		wrapWriteError(SET_TEXT_ATTRIBUTE)
	}

	void Console::resetAttributes() {
		try {
			file.write("\033[m", static_cast<MemorySize>(3u));
		}
		wrapWriteError(SET_TEXT_ATTRIBUTE)
	}

	void Console::setCursorPosition(unsigned row, unsigned column) {
		if(row >= 1000000u)
			row = 999999u;
		if(column >= 1000000u)
			column = 999999u;
		char seq[17];
		sprintf(seq, "\033[%u;%uH", row, column);
		try {
			file.write(seq, strlen(seq));
		}
		wrapWriteError(SET_CURSOR_POSITION)
	}

	void Console::getConsoleDimensions(unsigned& width, unsigned& height) {
		int fd;
		if(isatty(0))
			fd = 0;
		else if(isatty(1))
			fd = 1;
		else if(isatty(2))
			fd = 2;
		else {
			width = height = 0u;
			return;
		}
		struct winsize wsz;
		if(ioctl(fd, TIOCGWINSZ, &wsz))
			width = height = 0u;
		else {
			width = static_cast<unsigned>(wsz.ws_col);
			height = static_cast<unsigned>(wsz.ws_row);
		}
	}

	File::Handle Console::getStandardHandle(StandardHandle handle) {
		return static_cast<int>(handle);
	}

	bool Console::isConsole(File::Handle handle) {
		return !!isatty(handle);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	void Console::clear() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(!GetConsoleScreenBufferInfo(file.getHandle(), &csbi))
			throw ConsoleError(GetLastError(), ConsoleError::CLEAR);
		DWORD size = static_cast<DWORD>(csbi.dwSize.X) * static_cast<DWORD>(csbi.dwSize.Y), written;
		COORD startCoord;
		startCoord.X = startCoord.Y = static_cast<SHORT>(0);
		if(
			!FillConsoleOutputCharacter(file.getHandle(), static_cast<TCHAR>(' '), size, startCoord, &written)
			|| !FillConsoleOutputAttribute(file.getHandle(), csbi.wAttributes, size, startCoord, &written)
			|| !SetConsoleCursorPosition(file.getHandle(), startCoord)
		)
			throw ConsoleError(GetLastError(), ConsoleError::CLEAR);
	}

	void Console::home() {
		COORD startCoord;
		startCoord.X = startCoord.Y = static_cast<SHORT>(0);
		if(!SetConsoleCursorPosition(file.getHandle(), startCoord))
			throw ConsoleError(GetLastError(), ConsoleError::SET_CURSOR_POSITION);
	}

#define FOREGROUND_BITS static_cast<WORD>(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define BACKGROUND_BITS static_cast<WORD>(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)

	static const WORD FOREGROUND_COLORS[8] = {
		static_cast<WORD>(0),
		static_cast<WORD>(FOREGROUND_RED),
		static_cast<WORD>(FOREGROUND_GREEN),
		static_cast<WORD>(FOREGROUND_RED | FOREGROUND_GREEN),
		static_cast<WORD>(FOREGROUND_BLUE),
		static_cast<WORD>(FOREGROUND_RED | FOREGROUND_BLUE),
		static_cast<WORD>(FOREGROUND_GREEN | FOREGROUND_BLUE),
		static_cast<WORD>(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
	};

	static const WORD BACKGROUND_COLORS[8] = {
		static_cast<WORD>(0),
		static_cast<WORD>(BACKGROUND_RED),
		static_cast<WORD>(BACKGROUND_GREEN),
		static_cast<WORD>(BACKGROUND_RED | BACKGROUND_GREEN),
		static_cast<WORD>(BACKGROUND_BLUE),
		static_cast<WORD>(BACKGROUND_RED | BACKGROUND_BLUE),
		static_cast<WORD>(BACKGROUND_GREEN | BACKGROUND_BLUE),
		static_cast<WORD>(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
	};

	void Console::setForeground(Color color) {
		if(color < 0 || color > 7)
			return;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(!GetConsoleScreenBufferInfo(file.getHandle(), &csbi))
			throw ConsoleError(GetLastError(), ConsoleError::SET_TEXT_ATTRIBUTE);
		WORD attrs = static_cast<WORD>(
			(csbi.wAttributes & static_cast<WORD>(~FOREGROUND_BITS))
			| FOREGROUND_COLORS[color]
		);
		if(!SetConsoleTextAttribute(file.getHandle(), attrs))
			throw ConsoleError(GetLastError(), ConsoleError::SET_TEXT_ATTRIBUTE);
	}

	void Console::setBackground(Color color) {
		if(color < 0 || color > 7)
			return;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(!GetConsoleScreenBufferInfo(file.getHandle(), &csbi))
			throw ConsoleError(GetLastError(), ConsoleError::SET_TEXT_ATTRIBUTE);
		WORD attrs = static_cast<WORD>(
			(csbi.wAttributes & static_cast<WORD>(~BACKGROUND_BITS))
			| BACKGROUND_COLORS[color]
		);
		if(!SetConsoleTextAttribute(file.getHandle(), attrs))
			throw ConsoleError(GetLastError(), ConsoleError::SET_TEXT_ATTRIBUTE);
	}

	void Console::setIntensity(Intensity intensity) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(!GetConsoleScreenBufferInfo(file.getHandle(), &csbi))
			throw ConsoleError(GetLastError(), ConsoleError::SET_TEXT_ATTRIBUTE);
		WORD attrs = static_cast<WORD>(csbi.wAttributes
				& static_cast<WORD>(~static_cast<WORD>(FOREGROUND_INTENSITY)));
		if(intensity != NORMAL)
			attrs = static_cast<WORD>(attrs | static_cast<WORD>(FOREGROUND_INTENSITY));
		if(!SetConsoleTextAttribute(file.getHandle(), attrs))
			throw ConsoleError(GetLastError(), ConsoleError::SET_TEXT_ATTRIBUTE);
	}

	void Console::resetAttributes() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(!GetConsoleScreenBufferInfo(file.getHandle(), &csbi))
			throw ConsoleError(GetLastError(), ConsoleError::SET_TEXT_ATTRIBUTE);
		WORD attrs = static_cast<WORD>((csbi.wAttributes & static_cast<WORD>(~static_cast<WORD>(
			FOREGROUND_BITS
			| BACKGROUND_BITS
			| static_cast<WORD>(FOREGROUND_INTENSITY)
		))) | FOREGROUND_COLORS[WHITE]);
		if(!SetConsoleTextAttribute(file.getHandle(), attrs))
			throw ConsoleError(GetLastError(), ConsoleError::SET_TEXT_ATTRIBUTE);
	}

	void Console::setCursorPosition(unsigned row, unsigned column) {
		if(!row)
			row = 1u;
		else if(row >= static_cast<unsigned>(IntegerBounds<SHORT>::MAX))
			row = static_cast<unsigned>(IntegerBounds<SHORT>::MAX);
		if(!column)
			column = 1u;
		else if(column >= static_cast<unsigned>(IntegerBounds<SHORT>::MAX))
			column = static_cast<unsigned>(IntegerBounds<SHORT>::MAX);
		COORD coords;
		coords.X = static_cast<SHORT>(column - 1u);
		coords.Y = static_cast<SHORT>(row - 1u);
		if(!SetConsoleCursorPosition(file.getHandle(), coords))
			throw ConsoleError(GetLastError(), ConsoleError::SET_CURSOR_POSITION);
	}

	void Console::getConsoleDimensions(unsigned& width, unsigned& height) {
		HANDLE fd;
		if(Console::isConsole((fd = GetStdHandle(STD_INPUT_HANDLE))))
			{}
		else if(Console::isConsole((fd = GetStdHandle(STD_OUTPUT_HANDLE))))
			{}
		else if(Console::isConsole((fd = GetStdHandle(STD_ERROR_HANDLE))))
			{}
		else {
			width = height = 0u;
			return;
		}
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(GetConsoleScreenBufferInfo(fd, &csbi)) {
			width = static_cast<unsigned>(csbi.srWindow.Right - csbi.srWindow.Left + static_cast<SHORT>(1));
			height = static_cast<unsigned>(csbi.srWindow.Bottom - csbi.srWindow.Top + static_cast<SHORT>(1));
		}
		else
			width = height = 0u;
	}

	File::Handle Console::getStandardHandle(StandardHandle handle) {
		switch(handle) {
			case STANDARD_INPUT:
				return GetStdHandle(STD_INPUT_HANDLE);
			case STANDARD_OUTPUT:
				return GetStdHandle(STD_OUTPUT_HANDLE);
			case STANDARD_ERROR:
				return GetStdHandle(STD_ERROR_HANDLE);
			default:
				return File::INVALID_HANDLE;
		}
	}

	bool Console::isConsole(File::Handle handle) {
		DWORD mode;
		return !!GetConsoleMode(handle, &mode);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}
