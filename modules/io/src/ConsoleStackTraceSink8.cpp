#include "ConsoleStackTraceSink8.hpp"

using redengine::platform::File;
using redengine::platform::Console;

namespace redengine {
namespace io {

	ConsoleStackTraceSink8::ConsoleStackTraceSink8(Console::StandardHandle handle)
			: ConsoleOutputStreamStackTraceSink8(stream, indenter, NULL), stream(Console::getStandardHandle(handle)),
			indenter(stream), console(handle) {
		stream.setCloseOnDestroy(false);
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleStackTraceSink8::ConsoleStackTraceSink8(const ConsoleStackTraceSink8& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			ConsoleOutputStreamStackTraceSink8(stream, indenter, NULL),
			stream(sink.stream.getFile().getHandle(), File::OUTPUT), indenter(stream), console(sink.console) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleStackTraceSink8::~ConsoleStackTraceSink8() {}

}}
