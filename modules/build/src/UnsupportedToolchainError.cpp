#include "UnsupportedToolchainError.hpp"

using std::string;

namespace redengine {
namespace build {

	UnsupportedToolchainError::UnsupportedToolchainError(const string& toolchain) : toolchain(toolchain) {}

	UnsupportedToolchainError::UnsupportedToolchainError(const UnsupportedToolchainError& error)
			: BuildError(error), toolchain(error.toolchain) {}

	REDSTRAIN_DEFINE_ERROR(UnsupportedToolchainError) {
		out << "Toolchain not supported: " << toolchain;
	}

}}
