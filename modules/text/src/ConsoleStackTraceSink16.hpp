#ifndef REDSTRAIN_MOD_TEXT_CONSOLESTACKTRACESINK16_HPP
#define REDSTRAIN_MOD_TEXT_CONSOLESTACKTRACESINK16_HPP

#include "OutputStreamStackTraceIndenter16.hpp"
#include "ConsoleOutputStreamStackTraceSink16.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API ConsoleStackTraceSink16 : public ConsoleOutputStreamStackTraceSink16 {

	  private:
		OutputStreamStackTraceIndenter16 indenter;
		platform::Console console;

	  public:
		ConsoleStackTraceSink16(platform::Console::StandardHandle);
		ConsoleStackTraceSink16(const ConsoleStackTraceSink16&);
		virtual ~ConsoleStackTraceSink16();

		inline OutputStreamStackTraceIndenter16& getIndenter() {
			return indenter;
		}

		inline const OutputStreamStackTraceIndenter16& getIndenter() const {
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

#endif /* REDSTRAIN_MOD_TEXT_CONSOLESTACKTRACESINK16_HPP */
