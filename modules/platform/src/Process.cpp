#include <cstdlib>

#include "Thread.hpp"
#include "ProcessOperationError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif /* OS-specific includes */

using std::map;
using std::string;

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
extern char** environ;
#endif /* OS-specific declarations */

namespace redengine {
namespace platform {

	Process::Process(Handle handle) : handle(handle), stdinHandle(File::INVALID_HANDLE),
			stdoutHandle(File::INVALID_HANDLE), stderrHandle(File::INVALID_HANDLE) {}

	Process::Process(Handle handle, File::Handle stdinHandle, File::Handle stdoutHandle, File::Handle stderrHandle)
			: handle(handle), stdinHandle(stdinHandle), stdoutHandle(stdoutHandle), stderrHandle(stderrHandle) {}

	Process::Process(const Process& process) : handle(process.handle), stdinHandle(process.stdinHandle),
			stdoutHandle(process.stdoutHandle), stderrHandle(process.stderrHandle) {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	static void killWith(pid_t pid, int signal, ProcessOperationError::Operation operation) {
		if(kill(pid, signal))
			throw ProcessOperationError(operation, errno);
	}

	void Process::terminate() {
		killWith(handle, SIGTERM, ProcessOperationError::TERMINATE);
	}

	void Process::kill() {
		killWith(handle, SIGKILL, ProcessOperationError::KILL);
	}

	void Process::interrupt() {
		killWith(handle, SIGINT, ProcessOperationError::INTERRUPT);
	}

	int Process::wait() {
		int status;
		if(waitpid(handle, &status, 0) == static_cast<pid_t>(-1))
			throw ProcessOperationError(ProcessOperationError::WAIT, errno);
		return status;
	}

	void Process::getenv(map<string, string>& drop) {
		drop.clear();
		char** e = ::environ;
		for(; *e; ++e) {
			string piece(*e);
			string::size_type pos = piece.find('=');
			if(pos == string::npos)
				continue;
			drop[piece.substr(static_cast<string::size_type>(0u), pos)]
					= piece.substr(pos + static_cast<string::size_type>(1u));
		}
	}

	string Process::resolveExecutable(const string& name) {
		if(name.find('/') != string::npos)
			return name;
		const char* path = ::getenv("PATH");
		if(!path)
			return "";
		string ps(path);
		string::size_type pos, old = static_cast<string::size_type>(0u);
		while((pos = ps.find(':', old)) != string::npos) {
			string piece(ps.substr(old, pos - old));
			old = pos + static_cast<string::size_type>(1u);
			if(piece.empty())
				continue;
			piece += '/';
			piece += name;
			if(!access(piece.c_str(), X_OK))
				return piece;
		}
		return "";
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	void Process::terminate() {
		//TODO
	}

	void Process::kill() {
		//TODO
	}

	void Process::interrupt() {
		//TODO
	}

	int Process::wait() {
		//TODO
		return 0;
	}

	void Process::getenv(map<string, string>& drop) {
		drop.clear();
		//TODO
	}

	string Process::resolveExecutable(const string& name) {
		//TODO
		return "";
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

	string Process::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(error);
	}

	const char* Process::getenv(const string& key) {
		return ::getenv(key.c_str());
	}

	void Process::putenv(const string& key, const string& value) {
		putenv(key.c_str(), value.c_str());
	}

}}
