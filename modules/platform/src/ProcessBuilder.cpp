#include <cstring>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "ProcessBuilder.hpp"
#include "ProcessOperationError.hpp"
#include "IllegalStandardStreamSpecifierError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <fcntl.h>
#endif /* OS-specific includes */

using std::map;
using std::list;
using std::vector;
using std::string;
using redengine::util::MemorySize;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;
using redengine::error::IllegalArgumentError;

namespace redengine {
namespace platform {

	ProcessBuilder::ProcessBuilder() : hasEnv(false), stdinMode(STREAM_INHERITED), stdoutMode(STREAM_INHERITED),
			stderrMode(STREAM_INHERITED), stdinFile(File::INVALID_HANDLE), stdoutFile(File::INVALID_HANDLE),
			stderrFile(File::INVALID_HANDLE), joinedStreams(false) {}

	ProcessBuilder::ProcessBuilder(const list<string>& arguments) : arguments(arguments), hasEnv(false),
			stdinMode(STREAM_INHERITED), stdoutMode(STREAM_INHERITED), stderrMode(STREAM_INHERITED),
			stdinFile(File::INVALID_HANDLE), stdoutFile(File::INVALID_HANDLE), stderrFile(File::INVALID_HANDLE),
			joinedStreams(false) {}

	ProcessBuilder::ProcessBuilder(const vector<string>& arguments) : hasEnv(false), stdinMode(STREAM_INHERITED),
			stdoutMode(STREAM_INHERITED), stderrMode(STREAM_INHERITED), stdinFile(File::INVALID_HANDLE),
			stdoutFile(File::INVALID_HANDLE), stderrFile(File::INVALID_HANDLE), joinedStreams(false) {
		this->arguments.insert(this->arguments.end(), arguments.begin(), arguments.end());
	}

	ProcessBuilder::ProcessBuilder(const char *const* arguments) : hasEnv(false), stdinMode(STREAM_INHERITED),
			stdoutMode(STREAM_INHERITED), stderrMode(STREAM_INHERITED), stdinFile(File::INVALID_HANDLE),
			stdoutFile(File::INVALID_HANDLE), stderrFile(File::INVALID_HANDLE), joinedStreams(false) {
		for(; *arguments; ++arguments)
			this->arguments.push_back(*arguments);
	}

	ProcessBuilder::ProcessBuilder(const char *const* arguments, MemorySize count) : hasEnv(false),
			stdinMode(STREAM_INHERITED), stdoutMode(STREAM_INHERITED), stderrMode(STREAM_INHERITED),
			stdinFile(File::INVALID_HANDLE), stdoutFile(File::INVALID_HANDLE), stderrFile(File::INVALID_HANDLE),
			joinedStreams(false) {
		this->arguments.insert(this->arguments.end(), arguments, arguments + count);
	}

	ProcessBuilder::ProcessBuilder(const ProcessBuilder& builder) : arguments(builder.arguments),
			targetEnv(builder.targetEnv), hasEnv(builder.hasEnv), stdinMode(builder.stdinMode),
			stdoutMode(builder.stdoutMode), stderrMode(builder.stderrMode), stdinFile(builder.stdinFile),
			stdoutFile(builder.stdoutFile), stderrFile(builder.stderrFile), joinedStreams(builder.joinedStreams),
			workdir(builder.workdir) {}

	void ProcessBuilder::addArgument(const string& argument) {
		arguments.push_back(argument);
	}

	void ProcessBuilder::addArguments(const list<string>& arguments) {
		this->arguments.insert(this->arguments.end(), arguments.begin(), arguments.end());
	}

	void ProcessBuilder::addArguments(const vector<string>& arguments) {
		this->arguments.insert(this->arguments.end(), arguments.begin(), arguments.end());
	}

	void ProcessBuilder::addArguments(const char *const* arguments) {
		for(; *arguments; ++arguments)
			this->arguments.push_back(*arguments);
	}

	void ProcessBuilder::addArguments(const char *const* arguments, MemorySize count) {
		this->arguments.insert(this->arguments.end(), arguments, arguments + count);
	}

	MemorySize ProcessBuilder::getArgumentCount() const {
		return static_cast<MemorySize>(arguments.size());
	}

	bool ProcessBuilder::hasArguments() const {
		return !arguments.empty();
	}

	void ProcessBuilder::getArguments(ArgumentIterator& begin, ArgumentIterator& end) const {
		begin = arguments.begin();
		end = arguments.end();
	}

	void ProcessBuilder::setEnvironment() {
		hasEnv = true;
		targetEnv.clear();
	}

	void ProcessBuilder::setEnvironment(const map<string, string>& environment) {
		hasEnv = true;
		targetEnv = environment;
	}

	void ProcessBuilder::clearEnvironment() {
		hasEnv = false;
		targetEnv.clear();
	}

	void ProcessBuilder::putEnvironment(const string& key, const string& value) {
		hasEnv = true;
		targetEnv[key] = value;
	}

	void ProcessBuilder::setStreamInherited(Console::StandardHandle stream) {
		#define clamp(which) \
			which ## Mode = STREAM_INHERITED; \
			which ## File = File::INVALID_HANDLE; \
			break;
		switch(stream) {
			case Console::STANDARD_INPUT:
				clamp(stdin)
			case Console::STANDARD_OUTPUT:
				joinedStreams = false;
				clamp(stdout)
			case Console::STANDARD_ERROR:
				joinedStreams = false;
				clamp(stderr)
			default:
				throw IllegalStandardStreamSpecifierError(stream);
		}
		#undef clamp
	}

	void ProcessBuilder::setStreamPipe(Console::StandardHandle stream) {
		#define clamp(which) \
			which ## Mode = STREAM_PIPE; \
			which ## File = File::INVALID_HANDLE; \
			break;
		switch(stream) {
			case Console::STANDARD_INPUT:
				clamp(stdin)
			case Console::STANDARD_OUTPUT:
				joinedStreams = false;
				clamp(stdout)
			case Console::STANDARD_ERROR:
				joinedStreams = false;
				clamp(stderr)
			default:
				throw IllegalStandardStreamSpecifierError(stream);
		}
		#undef clamp
	}

	void ProcessBuilder::setStreamFile(Console::StandardHandle stream, const File& file) {
		#define clamp(which) \
			which ## Mode = STREAM_FILE; \
			which ## File = file.getHandle(); \
			break;
		switch(stream) {
			case Console::STANDARD_INPUT:
				clamp(stdin)
			case Console::STANDARD_OUTPUT:
				joinedStreams = false;
				clamp(stdout)
			case Console::STANDARD_ERROR:
				joinedStreams = false;
				clamp(stderr)
			default:
				throw IllegalStandardStreamSpecifierError(stream);
		}
		#undef clamp
	}

	void ProcessBuilder::setStreamFile(Console::StandardHandle stream, File::Handle file) {
		#define clamp(which) \
			which ## Mode = STREAM_FILE; \
			which ## File = file; \
			break;
		switch(stream) {
			case Console::STANDARD_INPUT:
				clamp(stdin)
			case Console::STANDARD_OUTPUT:
				joinedStreams = false;
				clamp(stdout)
			case Console::STANDARD_ERROR:
				joinedStreams = false;
				clamp(stderr)
			default:
				throw IllegalStandardStreamSpecifierError(stream);
		}
		#undef clamp
	}

	ProcessBuilder::StreamMode ProcessBuilder::getStreamMode(Console::StandardHandle stream) const {
		#define clamp(which) \
			return which ## Mode;
		switch(stream) {
			case Console::STANDARD_INPUT:
				clamp(stdin)
			case Console::STANDARD_OUTPUT:
				clamp(stdout)
			case Console::STANDARD_ERROR:
				clamp(stderr)
			default:
				throw IllegalStandardStreamSpecifierError(stream);
		}
		#undef clamp
	}

	File::Handle ProcessBuilder::getStreamFile(Console::StandardHandle stream) const {
		#define clamp(which) \
			return which ## File;
		switch(stream) {
			case Console::STANDARD_INPUT:
				clamp(stdin)
			case Console::STANDARD_OUTPUT:
				clamp(stdout)
			case Console::STANDARD_ERROR:
				clamp(stderr)
			default:
				throw IllegalStandardStreamSpecifierError(stream);
		}
		#undef clamp
	}

	void ProcessBuilder::joinOutputStreams() {
		joinedStreams = true;
		stderrMode = stdoutMode;
		stderrFile = stdoutFile;
	}

	bool ProcessBuilder::areOutputStreamsJoined() const {
		if(joinedStreams)
			return true;
		return stderrMode == STREAM_FILE && stdoutMode == STREAM_FILE && stderrFile == stdoutFile;
	}

	void ProcessBuilder::setWorkingDirectory() {
		workdir.clear();
	}

	void ProcessBuilder::setWorkingDirectory(const string& workingDirectory) {
		workdir = workingDirectory;
	}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

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

	Process ProcessBuilder::execute() const {
		size_t vsize = static_cast<size_t>(arguments.size());
		char** bareArgv = new char*[vsize + static_cast<size_t>(1u)];
		DeleteArgv delArgv(bareArgv);
		list<string>::const_iterator it(arguments.begin());
		for(; delArgv.size < vsize; ++it) {
			char* buf = new char[static_cast<size_t>(it->size()) + static_cast<size_t>(1u)];
			bareArgv[delArgv.size] = buf;
			++delArgv.size;
			strcpy(buf, it->c_str());
		}
		bareArgv[vsize] = NULL;
		return execByArgv(bareArgv);
	}

	Process ProcessBuilder::execByArgv(char** argv) const {
		char** bareEnv;
		DeleteArgv delEnv(NULL);
		if(hasEnv) {
			size_t msize = static_cast<size_t>(targetEnv.size());
			bareEnv = new char*[msize + static_cast<size_t>(1u)];
			delEnv.argv = bareEnv;
			map<string, string>::const_iterator it(targetEnv.begin());
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

	static void prepareStream(ProcessBuilder::StreamMode mode, int* pipes) {
		switch(mode) {
			case ProcessBuilder::STREAM_INHERITED:
				break;
			case ProcessBuilder::STREAM_PIPE:
				if(pipe(pipes))
					throw ProcessOperationError(ProcessOperationError::START, errno);
				break;
			case ProcessBuilder::STREAM_FILE:
				// will be duplicated in redirectStreamChild()
				break;
			default:
				throw ProgrammingError("Illegal standard stream mode: "
						+ StringUtils::toString(static_cast<int>(mode)));
		}
	}

	static int redirectStreamParent(ProcessBuilder::StreamMode mode,
			CloseFD streamPipeParentEnd, CloseFD& streamPipeChildEnd) {
		int fd;
		switch(mode) {
			case ProcessBuilder::STREAM_INHERITED:
				return -1;
			case ProcessBuilder::STREAM_PIPE:
				if(close(streamPipeChildEnd.fd))
					throw ProcessOperationError(ProcessOperationError::START, errno);
				streamPipeChildEnd.fd = -1;
				fd = streamPipeParentEnd.fd;
				streamPipeParentEnd.fd = -1;
				return fd;
			case ProcessBuilder::STREAM_FILE:
				return -1;
			default:
				throw ProgrammingError("Illegal standard stream mode: "
						+ StringUtils::toString(static_cast<int>(mode)));
		}
	}

	static void redirectStreamChild(int targetFD, ProcessBuilder::StreamMode mode, int handle,
			int streamPipeChildEnd, int streamPipeParentEnd, int controlPipe) {
		switch(mode) {
			case ProcessBuilder::STREAM_INHERITED:
				break;
			case ProcessBuilder::STREAM_PIPE:
				if(close(streamPipeParentEnd))
					postChildError(controlPipe);
				if(close(targetFD))
					postChildError(controlPipe);
				if(dup(streamPipeChildEnd) == -1)
					postChildError(controlPipe);
				if(close(streamPipeChildEnd))
					postChildError(controlPipe);
				break;
			case ProcessBuilder::STREAM_FILE:
				if(close(targetFD))
					postChildError(controlPipe);
				if(dup(handle) == -1)
					postChildError(controlPipe);
				if(close(handle))
					postChildError(controlPipe);
				break;
			default:
				throw ProgrammingError("Illegal standard stream mode: "
						+ StringUtils::toString(static_cast<int>(mode)));
		}
	}

	Process ProcessBuilder::execByPointers(char** argv, char** envp) const {
		string binfile;
		if(*argv)
			binfile = Process::resolveExecutable(*argv);
		if(binfile.empty())
			throw ProcessOperationError(ProcessOperationError::START, ENOENT);
		// prepare stream redirection
		int strpipes[] = {-1, -1, -1, -1, -1, -1};
		prepareStream(stdinMode, strpipes);
		CloseFD closeStdinRead(*strpipes), closeStdinWrite(strpipes[1]);
		prepareStream(stdoutMode, strpipes + 2);
		CloseFD closeStdoutRead(strpipes[2]), closeStdoutWrite(strpipes[3]);
		if(!joinedStreams)
			prepareStream(stderrMode, strpipes + 4);
		CloseFD closeStderrRead(strpipes[4]), closeStderrWrite(strpipes[5]);
		// set up control pipe
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
		// fork
		pid_t pid = fork();
		if(pid == static_cast<pid_t>(-1))
			throw ProcessOperationError(ProcessOperationError::START, errno);
		int procStdin = -1, procStdout = -1, procStderr = -1;
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
			// redirect streams
			CloseFD stdinHandle(redirectStreamParent(stdinMode, closeStdinWrite, closeStdinRead));
			CloseFD stdoutHandle(redirectStreamParent(stdoutMode, closeStdoutRead, closeStdoutWrite));
			CloseFD stderrHandle(-1);
			if(!joinedStreams)
				stderrHandle.fd = redirectStreamParent(stderrMode, closeStderrRead, closeStderrWrite);
			// read error
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
			// finished
			procStdin = stdinHandle.fd;
			stdinHandle.fd = -1;
			procStdout = stdoutHandle.fd;
			stdoutHandle.fd = -1;
			procStderr = stderrHandle.fd;
			stderrHandle.fd = -1;
		}
		else {
			// child
			if(close(*pipefd))
				postChildError(pipefd[1]);
			if(!workdir.empty()) {
				if(chdir(workdir.c_str()))
					postChildError(pipefd[1]);
			}
			// redirect streams
			redirectStreamChild(0, stdinMode, stdinFile, *strpipes, strpipes[1], pipefd[1]);
			redirectStreamChild(1, stdoutMode, stdoutFile, strpipes[3], strpipes[2], pipefd[1]);
			if(joinedStreams) {
				if(close(2))
					postChildError(pipefd[1]);
				if(dup(1) == -1)
					postChildError(pipefd[1]);
			}
			else
				redirectStreamChild(2, stderrMode, stderrFile, strpipes[5], strpipes[4], pipefd[1]);
			execve(binfile.c_str(), argv, envp);
			postChildError(pipefd[1]);
		}
		return Process(pid, procStdin, procStdout, procStderr);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	Process ProcessBuilder::execute() const {
		//TODO
		return Process(0);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}
