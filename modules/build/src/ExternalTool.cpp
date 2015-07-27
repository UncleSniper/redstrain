#include <iostream>

#include "DebugMode.hpp"
#include "ExternalTool.hpp"

#include "ToolchainFailureError.hpp"

using std::cerr;
using std::endl;
using std::string;
using redengine::platform::Process;
using redengine::platform::ProcessBuilder;

namespace redengine {
namespace build {

	// ======== ExternalInvocation ========

	ExternalTool::ExternalInvocation::ExternalInvocation(const string& executable) {
		command.addArgument(executable);
	}

	ExternalTool::ExternalInvocation::ExternalInvocation(const ExternalInvocation& invocation)
			: Invocation(invocation), command(invocation.command) {}

	void ExternalTool::ExternalInvocation::checkStatus(int status) const {
		if(!status)
			return;
		ProcessBuilder::ArgumentIterator begin, end;
		command.getArguments(begin, end);
		string tool;
		if(begin != end)
			tool = *begin;
		throw ToolchainFailureError(tool, status);
	}

	void ExternalTool::ExternalInvocation::invoke() {
		if(DebugMode::getInstance().isDebug()) {
			cerr << "REDBUILD_DEBUG: Will execute:";
			ProcessBuilder::ArgumentIterator argbegin, argend;
			command.getArguments(argbegin, argend);
			for(; argbegin != argend; ++argbegin)
				cerr << " '" << *argbegin << '\'';
			cerr << endl;
		}
		Process proc = command.execute();
		checkStatus(proc.wait());
	}

	// ======== ExternalTool ========

	ExternalTool::ExternalTool(const string& executable) : executable(executable) {}

	ExternalTool::ExternalTool(const ExternalTool& tool) : executable(tool.executable) {}

	ExternalTool::~ExternalTool() {}

	void ExternalTool::setExecutable(const string& executable) {
		this->executable = executable;
	}

}}
