#include "ConsoleSymbolSink16.hpp"
#include "ConsoleOutputStream.hpp"

using redengine::platform::File;
using redengine::io::OutputStream;
using redengine::platform::Console;

namespace redengine {
namespace text {

	ConsoleSymbolSink16::ConsoleSymbolSink16(Console::StandardHandle handle)
			: ConsoleOutputStreamSymbolSink16(handle == Console::STANDARD_ERROR
					? getStandardErrorStream16() : getStandardOutputStream16(), indenter, NULL),
			indenter(getStream()), console(handle) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleSymbolSink16::ConsoleSymbolSink16(const ConsoleSymbolSink16& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink),
			ConsoleOutputStreamSymbolSink16(const_cast<OutputStream<Char16>&>(sink.getStream()), indenter, NULL),
			indenter(sink.indenter), console(sink.console) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleSymbolSink16::~ConsoleSymbolSink16() {}

}}
