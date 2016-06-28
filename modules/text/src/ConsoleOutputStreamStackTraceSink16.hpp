#ifndef REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMSTACKTRACESINK16_HPP
#define REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMSTACKTRACESINK16_HPP

#include <redstrain/platform/ConsoleHighlightingStackTraceSink.hpp>

#include "OutputStreamStackTraceSink16.hpp"

namespace redengine {
namespace text {

	class ConsoleOutputStreamSymbolSink16;

	class REDSTRAIN_TEXT_API ConsoleOutputStreamStackTraceSink16
			: public OutputStreamStackTraceSink16, public platform::ConsoleHighlightingStackTraceSink {

	  protected:
		virtual void writeSymbol(const redmond::unmangle::CPPSymbol&);
		virtual void configureConsoleSymbolSink(ConsoleOutputStreamSymbolSink16&, OutputStreamSymbolIndenter16&);

	  public:
		ConsoleOutputStreamStackTraceSink16(io::OutputStream<Char16>&, Indenter&, platform::Console*);
		ConsoleOutputStreamStackTraceSink16(const ConsoleOutputStreamStackTraceSink16&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMSTACKTRACESINK16_HPP */
