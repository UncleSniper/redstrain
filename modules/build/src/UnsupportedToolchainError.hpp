#ifndef REDSTRAIN_MOD_BUILD_UNSUPPORTEDTOOLCHAINERROR_HPP
#define REDSTRAIN_MOD_BUILD_UNSUPPORTEDTOOLCHAINERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API UnsupportedToolchainError : public BuildError {

	  private:
		const std::string toolchain;

	  public:
		UnsupportedToolchainError(const std::string&);
		UnsupportedToolchainError(const UnsupportedToolchainError&);

		inline const std::string& getToolchainName() const {
			return toolchain;
		}

		REDSTRAIN_DECLARE_ERROR(UnsupportedToolchainError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_UNSUPPORTEDTOOLCHAINERROR_HPP */
