#include "ToolchainFailureError.hpp"

using std::string;

namespace redengine {
namespace build {

	ToolchainFailureError::ToolchainFailureError(const string& tool, int status) : tool(tool), status(status) {}

	ToolchainFailureError::ToolchainFailureError(const ToolchainFailureError& error)
			: BuildError(error), tool(error.tool), status(error.status) {}

	REDSTRAIN_DEFINE_ERROR(ToolchainFailureError) {
		out << "Process '" << tool << "' terminated with status " << status;
	}

}}
