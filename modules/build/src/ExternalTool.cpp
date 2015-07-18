#include "ExternalTool.hpp"

#include "ToolchainFailureError.hpp"

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
