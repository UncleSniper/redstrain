#ifndef REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMERRORHANDLER16_HPP
#define REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMERRORHANDLER16_HPP

#include <redstrain/platform/ConsoleHighlightingErrorHandler.hpp>

#include "OutputStreamErrorHandler16.hpp"

namespace redengine {
namespace text {

	class ConsoleOutputStreamStackTraceSink16;

	class REDSTRAIN_TEXT_API ConsoleOutputStreamErrorHandler16
			: public OutputStreamErrorHandler16, public platform::ConsoleHighlightingErrorHandler {

	  protected:
		virtual void writeStackTrace(const error::StackTrace*);
		virtual void configureConsoleStackTraceSink(ConsoleOutputStreamStackTraceSink16&,
				OutputStreamStackTraceIndenter16&);

	  public:
		ConsoleOutputStreamErrorHandler16(io::OutputStream<Char16>&, Indenter&, platform::Console*);
		ConsoleOutputStreamErrorHandler16(const ConsoleOutputStreamErrorHandler16&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMERRORHANDLER16_HPP */
