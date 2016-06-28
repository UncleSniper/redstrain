#ifndef REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMSTACKTRACESINK8_HPP
#define REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMSTACKTRACESINK8_HPP

#include <redstrain/platform/ConsoleHighlightingStackTraceSink.hpp>

#include "OutputStreamStackTraceSink8.hpp"

namespace redengine {
namespace io {

	class ConsoleOutputStreamSymbolSink8;

	class REDSTRAIN_IO_API ConsoleOutputStreamStackTraceSink8
			: public OutputStreamStackTraceSink8, public platform::ConsoleHighlightingStackTraceSink {

	  protected:
		virtual void writeSymbol(const redmond::unmangle::CPPSymbol&);
		virtual void configureConsoleSymbolSink(ConsoleOutputStreamSymbolSink8&, OutputStreamSymbolIndenter8&);

	  public:
		ConsoleOutputStreamStackTraceSink8(OutputStream<char>&, Indenter&, platform::Console*);
		ConsoleOutputStreamStackTraceSink8(const ConsoleOutputStreamStackTraceSink8&);

	};

}}

#endif /* REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMSTACKTRACESINK8_HPP */
