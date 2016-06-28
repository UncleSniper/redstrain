#ifndef REDSTRAIN_MOD_IO_CONSOLESYMBOLSINK8_HPP
#define REDSTRAIN_MOD_IO_CONSOLESYMBOLSINK8_HPP

#include "FileOutputStream.hpp"
#include "OutputStreamSymbolIndenter8.hpp"
#include "ConsoleOutputStreamSymbolSink8.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API ConsoleSymbolSink8 : public ConsoleOutputStreamSymbolSink8 {

	  private:
		FileOutputStream stream;
		OutputStreamSymbolIndenter8 indenter;
		platform::Console console;

	  public:
		ConsoleSymbolSink8(platform::Console::StandardHandle);
		ConsoleSymbolSink8(const ConsoleSymbolSink8&);
		virtual ~ConsoleSymbolSink8();

		inline FileOutputStream& getFileStream() {
			return stream;
		}

		inline const FileOutputStream& getFileStream() const {
			return stream;
		}

		inline OutputStreamSymbolIndenter8& getIndenter() {
			return indenter;
		}

		inline const OutputStreamSymbolIndenter8& getIndenter() const {
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

#endif /* REDSTRAIN_MOD_IO_CONSOLESYMBOLSINK8_HPP */
