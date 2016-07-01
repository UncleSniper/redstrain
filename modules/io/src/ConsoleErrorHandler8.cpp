#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "ConsoleErrorHandler8.hpp"

using redengine::platform::File;
using redengine::platform::Console;
using redengine::platform::SynchronizedSingleton;

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

	class DefaultConsoleErrorHandler8Singleton : public SynchronizedSingleton<ConsoleErrorHandler8> {

	  protected:
		virtual ConsoleErrorHandler8* newInstance();

	  public:
		DefaultConsoleErrorHandler8Singleton();
		DefaultConsoleErrorHandler8Singleton(const DefaultConsoleErrorHandler8Singleton&);

	};

	DefaultConsoleErrorHandler8Singleton::DefaultConsoleErrorHandler8Singleton() {}

	DefaultConsoleErrorHandler8Singleton::DefaultConsoleErrorHandler8Singleton(const
			DefaultConsoleErrorHandler8Singleton& singleton)
			: SynchronizedSingleton<ConsoleErrorHandler8>(singleton) {}

	ConsoleErrorHandler8* DefaultConsoleErrorHandler8Singleton::newInstance() {
		return new ConsoleErrorHandler8(Console::STANDARD_ERROR);
	}

	static DefaultConsoleErrorHandler8Singleton defaultConsoleErrorHandler8Singleton;

	ConsoleErrorHandler8& ConsoleErrorHandler8::getDefaultErrorHandler() {
		return defaultConsoleErrorHandler8Singleton.get();
	}

}}