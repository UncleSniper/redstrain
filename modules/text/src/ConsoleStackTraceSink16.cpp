#include "ConsoleOutputStream.hpp"
#include "ConsoleStackTraceSink16.hpp"

using redengine::platform::File;
using redengine::io::OutputStream;
using redengine::platform::Console;

namespace redengine {
namespace text {

	ConsoleStackTraceSink16::ConsoleStackTraceSink16(Console::StandardHandle handle)
			: ConsoleOutputStreamStackTraceSink16(handle == Console::STANDARD_ERROR
					? getStandardErrorStream16() : getStandardOutputStream16(), indenter, NULL),
			indenter(getStream()), console(handle) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleStackTraceSink16::ConsoleStackTraceSink16(const ConsoleStackTraceSink16& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase16(sink),
			ConsoleOutputStreamStackTraceSink16(const_cast<OutputStream<Char16>&>(sink.getStream()), indenter, NULL),
			indenter(sink.indenter), console(sink.console) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleStackTraceSink16::~ConsoleStackTraceSink16() {}

}}
