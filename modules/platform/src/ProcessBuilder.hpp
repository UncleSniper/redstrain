#ifndef REDSTRAIN_MOD_PLATFORM_PROCESSBUILDER_HPP
#define REDSTRAIN_MOD_PLATFORM_PROCESSBUILDER_HPP

#include <map>
#include <list>
#include <vector>
#include <string>

#include "Process.hpp"
#include "Console.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ProcessBuilder {

	  public:
		typedef std::list<std::string>::const_iterator ArgumentIterator;

		enum StreamMode {
			STREAM_INHERITED,
			STREAM_PIPE,
			STREAM_FILE
		};

	  private:
		std::list<std::string> arguments;
		std::map<std::string, std::string> targetEnv;
		bool hasEnv;
		StreamMode stdinMode, stdoutMode, stderrMode;
		File::Handle stdinFile, stdoutFile, stderrFile;
		bool joinedStreams;
		std::string workdir;

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
	  private:
		Process execByArgv(char**) const;
		Process execByPointers(char**, char**) const;
#endif /* OS-specific helpers */

	  public:
		ProcessBuilder();
		ProcessBuilder(const std::list<std::string>&);
		ProcessBuilder(const std::vector<std::string>&);
		ProcessBuilder(const char *const*);
		ProcessBuilder(const char *const*, util::MemorySize);
		ProcessBuilder(const ProcessBuilder&);

		void addArgument(const std::string&);
		void addArguments(const std::list<std::string>&);
		void addArguments(const std::vector<std::string>&);
		void addArguments(const char *const*);
		void addArguments(const char *const*, util::MemorySize);
		util::MemorySize getArgumentCount() const;
		bool hasArguments() const;
		void getArguments(ArgumentIterator&, ArgumentIterator&) const;

		void setEnvironment();
		void setEnvironment(const std::map<std::string, std::string>&);
		void clearEnvironment();
		void putEnvironment(const std::string&, const std::string&);

		inline const std::map<std::string, std::string>& getEnvironment() const {
			return targetEnv;
		}

		inline bool hasEnvironment() const {
			return hasEnv;
		}

		void setStreamInherited(Console::StandardHandle);
		void setStreamPipe(Console::StandardHandle);
		void setStreamFile(Console::StandardHandle, const File&);
		void setStreamFile(Console::StandardHandle, File::Handle);
		StreamMode getStreamMode(Console::StandardHandle) const;
		File::Handle getStreamFile(Console::StandardHandle) const;
		void joinOutputStreams();
		bool areOutputStreamsJoined() const;

		void setWorkingDirectory();
		void setWorkingDirectory(const std::string&);

		inline const std::string& getWorkingDirectory() const {
			return workdir;
		}

		Process execute() const;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_PROCESSBUILDER_HPP */
