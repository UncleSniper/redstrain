#ifndef REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMERRORHANDLER8_HPP
#define REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMERRORHANDLER8_HPP

#include <redstrain/platform/ConsoleHighlightingErrorHandler.hpp>

#include "OutputStreamErrorHandler8.hpp"

namespace redengine {
namespace io {

	class ConsoleOutputStreamStackTraceSink8;

	class REDSTRAIN_IO_API ConsoleOutputStreamErrorHandler8
			: public OutputStreamErrorHandler8, public platform::ConsoleHighlightingErrorHandler {

	  protected:
		virtual void writeStackTrace(const error::StackTrace*);
		virtual void configureConsoleStackTraceSink(ConsoleOutputStreamStackTraceSink8&,
				OutputStreamStackTraceIndenter8&);

	  public:
		ConsoleOutputStreamErrorHandler8(OutputStream<char>&, Indenter&, platform::Console*);
		ConsoleOutputStreamErrorHandler8(const ConsoleOutputStreamErrorHandler8&);

	};

}}

#endif /* REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMERRORHANDLER8_HPP */
