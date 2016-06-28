#ifndef REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMSYMBOLSINK16_HPP
#define REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMSYMBOLSINK16_HPP

#include <redstrain/platform/ConsoleHighlightingSymbolSink.hpp>

#include "OutputStreamSymbolSink16.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API ConsoleOutputStreamSymbolSink16
			: public OutputStreamSymbolSink16, public platform::ConsoleHighlightingSymbolSink {

	  protected:
		virtual void notifyConsoleChanged();

	  public:
		ConsoleOutputStreamSymbolSink16(io::OutputStream<Char16>&, Indenter&, platform::Console*, int = 0);
		ConsoleOutputStreamSymbolSink16(const ConsoleOutputStreamSymbolSink16&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CONSOLEOUTPUTSTREAMSTREAMSYMBOLSINK16_HPP */
