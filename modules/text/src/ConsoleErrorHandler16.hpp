#ifndef REDSTRAIN_MOD_TEXT_CONSOLEERRORHANDLER16_HPP
#define REDSTRAIN_MOD_TEXT_CONSOLEERRORHANDLER16_HPP

#include "OutputStreamErrorIndenter16.hpp"
#include "ConsoleOutputStreamErrorHandler16.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API ConsoleErrorHandler16 : public ConsoleOutputStreamErrorHandler16 {

	  private:
		OutputStreamErrorIndenter16 indenter;
		platform::Console console;

	  public:
		ConsoleErrorHandler16(platform::Console::StandardHandle);
		ConsoleErrorHandler16(const ConsoleErrorHandler16&);
		virtual ~ConsoleErrorHandler16();

		inline OutputStreamErrorIndenter16& getIndenter() {
			return indenter;
		}

		inline const OutputStreamErrorIndenter16& getIndenter() const {
			return indenter;
		}

		inline platform::Console& getConsole() {
			return console;
		}

		inline const platform::Console& getConsole() const {
			return console;
		}

		static ConsoleErrorHandler16& getDefaultStdErrorHandler();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CONSOLEERRORHANDLER16_HPP */
