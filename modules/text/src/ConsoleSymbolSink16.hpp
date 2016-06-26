#ifndef REDSTRAIN_MOD_TEXT_CONSOLESYMBOLSINK16_HPP
#define REDSTRAIN_MOD_TEXT_CONSOLESYMBOLSINK16_HPP

#include "OutputStreamSymbolSink16.hpp"
#include "OutputStreamSymbolIndenter16.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API ConsoleSymbolSink16 : public OutputStreamSymbolSink16 {

	  private:
		OutputStreamSymbolIndenter16 indenter;
		platform::Console console;

	  public:
		ConsoleSymbolSink16(platform::Console::StandardHandle);
		ConsoleSymbolSink16(const ConsoleSymbolSink16&);
		virtual ~ConsoleSymbolSink16();

		inline OutputStreamSymbolIndenter16& getIndenter() {
			return indenter;
		}

		inline const OutputStreamSymbolIndenter16& getIndenter() const {
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

#endif /* REDSTRAIN_MOD_TEXT_CONSOLESYMBOLSINK16_HPP */
