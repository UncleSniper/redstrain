#include "Library.hpp"
#include "LibraryOpenError.hpp"
#include "LibraryCloseError.hpp"
#include "LibraryResolveError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <dlfcn.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include "Thread.hpp"
#endif /* OS-specific includes */

using std::string;

namespace redengine {
namespace platform {

	Library::Library(Handle handle) : handle(handle) {}

	Library::Library(const Library& library) : handle(library.handle) {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	const Library::Handle Library::INVALID_HANDLE = NULL;
	const char *const Library::FILENAME_PREFIX = "lib";
	const char *const Library::FILENAME_SUFFIX = ".so";

	Library::Library(const string& name) {
		handle = dlopen(name.c_str(), RTLD_NOW | RTLD_LOCAL);
		if(handle == INVALID_HANDLE) {
			const char* msg = dlerror();
			throw LibraryOpenError(msg ? msg : "Unknown error");
		}
	}

	void Library::close() {
		if(handle == INVALID_HANDLE)
			return;
		if(dlclose(handle)) {
			const char* msg = dlerror();
			throw LibraryCloseError(msg ? msg : "Unknown error");
		}
		handle = INVALID_HANDLE;
	}

	void* Library::resolve(const string& symbol) const {
		dlerror();
		void* addr = dlsym(handle, symbol.c_str());
		if(addr)
			return addr;
		const char* msg = dlerror();
		if(msg)
			throw LibraryResolveError(msg);
		return NULL;
	}

	void Library::loadGlobal(const string& name) {
		void* handle = dlopen(name.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if(handle == INVALID_HANDLE) {
			const char* msg = dlerror();
			throw LibraryOpenError(msg ? msg : "Unknown error");
		}
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	const Library::Handle Library::INVALID_HANDLE = static_cast<Library::Handle>(0u);
	const char *const Library::FILENAME_PREFIX = "";
	const char *const Library::FILENAME_SUFFIX = ".dll";

	Library::Library(const string& name) {
		handle = LoadLibrary(name.c_str());
		if(handle == INVALID_HANDLE)
			throw LibraryOpenError(Thread::getErrorMessage(GetLastError()));
	}

	void Library::close() {
		if(handle == INVALID_HANDLE)
			return;
		if(!FreeLibrary(handle))
			throw LibraryCloseError(Thread::getErrorMessage(GetLastError()));
		handle = INVALID_HANDLE;
	}

	void* Library::resolve(const string& symbol) const {
		SetLastError(static_cast<DWORD>(0u));
		FARPROC addr = GetProcAddress(handle, symbol.c_str());
		if(addr)
			return *reinterpret_cast<void**>(&addr);
		DWORD err = GetLastError();
		if(err)
			throw LibraryResolveError(Thread::getErrorMessage(err));
		return NULL;
	}

	void Library::loadGlobal(const string&) {}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}
