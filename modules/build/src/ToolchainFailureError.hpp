#ifndef REDSTRAIN_MOD_BUILD_TOOLCHAINFAILUREERROR_HPP
#define REDSTRAIN_MOD_BUILD_TOOLCHAINFAILUREERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ToolchainFailureError : public BuildError {

	  private:
		const std::string tool;
		const int status;

	  public:
		ToolchainFailureError(const std::string&, int);
		ToolchainFailureError(const ToolchainFailureError&);

		REDSTRAIN_DECLARE_ERROR(ToolchainFailureError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_TOOLCHAINFAILUREERROR_HPP */
