#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "ConsoleOutputStream.hpp"
#include "ConsoleErrorHandler16.hpp"

using redengine::util::Delete;
using redengine::platform::File;
using redengine::io::OutputStream;
using redengine::platform::Console;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace text {

	ConsoleErrorHandler16::ConsoleErrorHandler16(Console::StandardHandle handle)
			: ConsoleOutputStreamErrorHandler16(handle == Console::STANDARD_ERROR
					? getStandardErrorStream16() : getStandardOutputStream16(), indenter, NULL),
			indenter(getStream()), console(handle) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE
				&& Console::isConsole(console.getFile().getHandle()))
			setHighlightingConsole(&console);
	}

	ConsoleErrorHandler16::ConsoleErrorHandler16(const ConsoleErrorHandler16& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase16(handler),
			ConsoleOutputStreamErrorHandler16(const_cast<OutputStream<Char16>&>(handler.getStream()),
			indenter, NULL), indenter(handler.indenter), console(handler.console) {
		if(console.getFile().getHandle() != File::INVALID_HANDLE
				&& Console::isConsole(console.getFile().getHandle()))
			setHighlightingConsole(&console);
	}

	ConsoleErrorHandler16::~ConsoleErrorHandler16() {}

	class DefaultConsoleErrorHandler16Singleton : public SynchronizedSingleton<ConsoleErrorHandler16> {

	  private:
		const Console::StandardHandle handle;

	  protected:
		virtual ConsoleErrorHandler16* newInstance() {
			Delete<ConsoleErrorHandler16> handler(new ConsoleErrorHandler16(handle));
			// trigger notifyConsoleChanged() to update columnCount
			handler->setHighlightingConsole(handler->getHighlightingConsole());
			return handler.set();
		}

	  public:
		DefaultConsoleErrorHandler16Singleton(Console::StandardHandle handle) : handle(handle) {}

		DefaultConsoleErrorHandler16Singleton(const DefaultConsoleErrorHandler16Singleton& singleton)
				: SynchronizedSingleton<ConsoleErrorHandler16>(singleton), handle(singleton.handle) {}

	};

	static DefaultConsoleErrorHandler16Singleton stdErrSingleton(Console::STANDARD_ERROR);

	ConsoleErrorHandler16& ConsoleErrorHandler16::getDefaultStdErrorHandler() {
		return stdErrSingleton.get();
	}

}}
