#ifndef REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMSYMBOLSINK8_HPP
#define REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMSYMBOLSINK8_HPP

#include <redstrain/platform/ConsoleHighlightingSymbolSink.hpp>

#include "OutputStreamSymbolSink8.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API ConsoleOutputStreamSymbolSink8
			: public OutputStreamSymbolSink8, public platform::ConsoleHighlightingSymbolSink {

	  protected:
		virtual void notifyConsoleChanged();

	  public:
		ConsoleOutputStreamSymbolSink8(OutputStream<char>&, Indenter&, platform::Console*, int = 0);
		ConsoleOutputStreamSymbolSink8(const ConsoleOutputStreamSymbolSink8&);

	};

}}

#endif /* REDSTRAIN_MOD_IO_CONSOLEOUTPUTSTREAMSYMBOLSINK8_HPP */
