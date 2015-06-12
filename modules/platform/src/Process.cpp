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
using std::list;
using std::vector;
using std::string;

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
extern char** environ;
#endif /* OS-specific declarations */

namespace redengine {
namespace platform {

	Process::Process(Handle handle) : handle(handle) {}

	Process::Process(const Process& process) : handle(process.handle) {}

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

	struct DeleteArgv {

		char** argv;
		size_t size;

		DeleteArgv(char** argv) : argv(argv), size(static_cast<size_t>(0u)) {}

		~DeleteArgv() {
			if(!argv)
				return;
			size_t u;
			for(u = static_cast<size_t>(0u); u < size; ++u)
				delete[] argv[u];
			delete[] argv;
		}

	};

	struct CloseFD {

		int fd;

		CloseFD(int fd) : fd(fd) {}

		~CloseFD() {
			if(fd != -1)
				close(fd);
		}

	};

	static void postChildError(int fd) {
		int code = errno;
		const char *begin = reinterpret_cast<char*>(&code), *end = reinterpret_cast<char*>(&code + 1);
		while(begin < end) {
			ssize_t count = write(fd, begin, static_cast<size_t>(end - begin));
			if(count == static_cast<ssize_t>(-1))
				break;
			begin += static_cast<size_t>(count);
		}
		_exit(1);
	}

	static pid_t execByPointers(char** argv, char** envp) {
		string binfile;
		if(*argv)
			binfile = Process::resolveExecutable(*argv);
		if(binfile.empty())
			throw ProcessOperationError(ProcessOperationError::START, ENOENT);
		int pipefd[2];
		if(pipe(pipefd))
			throw ProcessOperationError(ProcessOperationError::START, errno);
		CloseFD closeRead(*pipefd), closeWrite(pipefd[1]);
		// Technically, there is a race condition here; if another thread
		// uses fork(2), the pipe file handles might be leaked. However,
		// relying on the presence of pipe2(2) is very Linux-specific and
		// should be avoided. That said, let's face it: A great many file
		// handles are liable to be leaked that way, so what the hell...
		if(fcntl(pipefd[1], F_SETFD, FD_CLOEXEC))
			throw ProcessOperationError(ProcessOperationError::START, errno);
		pid_t pid = fork();
		if(pid == static_cast<pid_t>(-1))
			throw ProcessOperationError(ProcessOperationError::START, errno);
		if(pid) {
			// parent
			if(close(pipefd[1]))
				// Aborting now would be bogus: The child is still going...
				// On the other hand, ignoring the failed close will cause
				// the read loop to hang, so we're boned either way. We
				// posit that a spurious exception is better than a hang,
				// so we'll throw anyway...
				throw ProcessOperationError(ProcessOperationError::START, errno);
			closeWrite.fd = -1;
			int code = 0;
			char *begin = reinterpret_cast<char*>(&code), *end = reinterpret_cast<char*>(&code + 1);
			while(begin < end) {
				ssize_t count = read(*pipefd, begin, static_cast<size_t>(end - begin));
				if(count == static_cast<ssize_t>(-1))
					throw ProcessOperationError(ProcessOperationError::START, errno);
				if(!count)
					break;
			}
			if(code)
				throw ProcessOperationError(ProcessOperationError::START, code);
		}
		else {
			// child
			if(close(*pipefd))
				postChildError(pipefd[1]);
			execve(binfile.c_str(), argv, envp);
			postChildError(pipefd[1]);
		}
		return pid;
	}

	static pid_t execByArgv(char** argv, const map<string, string>* envp) {
		char** bareEnv;
		DeleteArgv delEnv(NULL);
		if(envp) {
			size_t msize = static_cast<size_t>(envp->size());
			bareEnv = new char*[msize + static_cast<size_t>(1u)];
			delEnv.argv = bareEnv;
			map<string, string>::const_iterator it(envp->begin());
			for(; delEnv.size < msize; ++it) {
				size_t ksize = static_cast<size_t>(it->first.size());
				size_t vsize = static_cast<size_t>(it->second.size());
				char* buf = new char[ksize + vsize + static_cast<size_t>(2u)];
				bareEnv[delEnv.size] = buf;
				++delEnv.size;
				strcpy(buf, it->first.c_str());
				buf += ksize;
				*buf = '=';
				strcpy(buf + 1, it->second.c_str());
			}
			bareEnv[msize] = NULL;
		}
		else
			bareEnv = ::environ;
		return execByPointers(argv, bareEnv);
	}

	template<typename ListT>
	pid_t execByCollections(const ListT& argv, const map<string, string>* envp) {
		size_t vsize = static_cast<size_t>(argv.size());
		char** bareArgv = new char*[vsize + static_cast<size_t>(1u)];
		DeleteArgv delArgv(bareArgv);
		typename ListT::const_iterator it(argv.begin());
		for(; delArgv.size < vsize; ++it) {
			char* buf = new char[static_cast<size_t>(it->size()) + static_cast<size_t>(1u)];
			bareArgv[delArgv.size] = buf;
			++delArgv.size;
			strcpy(buf, it->c_str());
		}
		bareArgv[vsize] = NULL;
		return execByArgv(bareArgv, envp);
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

	template<typename ListT>
	DWORD execByCollections(const ListT& argv, const map<string, string>* envp) {
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

	Process Process::run(const list<string>& argv, const map<string, string>& envp) {
		return Process::run(argv, &envp);
	}

	Process Process::run(const vector<string>& argv, const map<string, string>& envp) {
		return Process::run(argv, &envp);
	}

	Process Process::run(const list<string>& argv, const map<string, string>* envp) {
		return Process(execByCollections(argv, envp));
	}

	Process Process::run(const vector<string>& argv, const map<string, string>* envp) {
		return Process(execByCollections(argv, envp));
	}

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
