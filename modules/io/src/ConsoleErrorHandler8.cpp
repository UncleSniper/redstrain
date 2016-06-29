#include "ConsoleErrorHandler8.hpp"

using redengine::platform::File;
using redengine::platform::Console;

namespace redengine {
namespace io {

	ConsoleErrorHandler8::ConsoleErrorHandler8(Console::StandardHandle handle)
			: ConsoleOutputStreamErrorHandler8(stream, indenter, NULL), stream(Console::getStandardHandle(handle)),
			indenter(stream), console(handle) {
		stream.setCloseOnDestroy(false);
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleErrorHandler8::ConsoleErrorHandler8(const ConsoleErrorHandler8& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ConsoleOutputStreamErrorHandler8(stream, indenter, NULL),
			stream(handler.stream.getFile().getHandle(), File::OUTPUT), indenter(stream), console(handler.console) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleErrorHandler8::~ConsoleErrorHandler8() {}

}}
