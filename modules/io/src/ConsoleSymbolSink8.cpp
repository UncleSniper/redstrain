#include "ConsoleSymbolSink8.hpp"

using redengine::platform::File;
using redengine::platform::Console;

namespace redengine {
namespace io {

	ConsoleSymbolSink8::ConsoleSymbolSink8(Console::StandardHandle handle)
			: ConsoleOutputStreamSymbolSink8(stream, indenter, NULL), stream(Console::getStandardHandle(handle)),
			indenter(stream), console(handle) {
		stream.setCloseOnDestroy(false);
		if(console.getFile().getHandle() != File::INVALID_HANDLE
				&& Console::isConsole(console.getFile().getHandle()))
			setConsole(&console);
	}

	ConsoleSymbolSink8::ConsoleSymbolSink8(const ConsoleSymbolSink8& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), ConsoleOutputStreamSymbolSink8(stream, indenter, NULL),
			stream(sink.stream.getFile().getHandle(), File::OUTPUT), indenter(stream), console(sink.console) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE
				&& Console::isConsole(console.getFile().getHandle()))
			setConsole(&console);
	}

	ConsoleSymbolSink8::~ConsoleSymbolSink8() {}

}}
