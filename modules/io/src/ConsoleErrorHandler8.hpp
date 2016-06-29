#ifndef REDSTRAIN_MOD_IO_CONSOLEERRORHANDLER8_HPP
#define REDSTRAIN_MOD_IO_CONSOLEERRORHANDLER8_HPP

#include "FileOutputStream.hpp"
#include "OutputStreamErrorIndenter8.hpp"
#include "ConsoleOutputStreamErrorHandler8.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API ConsoleErrorHandler8 : public ConsoleOutputStreamErrorHandler8 {

	  private:
		FileOutputStream stream;
		OutputStreamErrorIndenter8 indenter;
		platform::Console console;

	  public:
		ConsoleErrorHandler8(platform::Console::StandardHandle);
		ConsoleErrorHandler8(const ConsoleErrorHandler8&);
		virtual ~ConsoleErrorHandler8();

		inline FileOutputStream& getFileStream() {
			return stream;
		}

		inline const FileOutputStream& getFileStream() const {
			return stream;
		}

		inline OutputStreamErrorIndenter8& getIndenter() {
			return indenter;
		}

		inline const OutputStreamErrorIndenter8& getIndenter() const {
			return indenter;
		}

		inline platform::Console& getConsole() {
			return console;
		}

		inline const platform::Console& getConsole() const {
			return console;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_CONSOLEERRORHANDLER8_HPP */
