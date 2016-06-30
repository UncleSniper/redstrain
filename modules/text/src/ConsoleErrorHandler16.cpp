#include "ConsoleOutputStream.hpp"
#include "ConsoleErrorHandler16.hpp"

using redengine::platform::File;
using redengine::io::OutputStream;
using redengine::platform::Console;

namespace redengine {
namespace text {

	ConsoleErrorHandler16::ConsoleErrorHandler16(Console::StandardHandle handle)
			: ConsoleOutputStreamErrorHandler16(handle == Console::STANDARD_ERROR
					? getStandardErrorStream16() : getStandardOutputStream16(), indenter, NULL),
			indenter(getStream()), console(handle) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleErrorHandler16::ConsoleErrorHandler16(const ConsoleErrorHandler16& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase16(handler),
			ConsoleOutputStreamErrorHandler16(const_cast<OutputStream<Char16>&>(handler.getStream()),
			indenter, NULL), indenter(handler.indenter), console(handler.console) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE)
			setConsole(&console);
	}

	ConsoleErrorHandler16::~ConsoleErrorHandler16() {}

}}
