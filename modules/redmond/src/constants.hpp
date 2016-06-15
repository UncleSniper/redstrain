#ifndef REDSTRAIN_MOD_REDMOND_CONSTANTS_HPP
#define REDSTRAIN_MOD_REDMOND_CONSTANTS_HPP

#include "api.hpp"

namespace redengine {
namespace redmond {

	enum REDSTRAIN_REDMOND_API OperatingSystem {
		OS_LINUX,
		OS_WINDOWS,
		OS_UNKNOWN
	};

	enum REDSTRAIN_REDMOND_API CompilerFlavor {
		COMP_GCC,
		COMP_MSVC,
		COMP_UNKNOWN
	};

	enum REDSTRAIN_REDMOND_API Architecture {
		ARCH_I686,
		ARCH_X86_64,
		ARCH_UNKNOWN
	};

	REDSTRAIN_REDMOND_API extern const OperatingSystem buildHostOS;
	REDSTRAIN_REDMOND_API extern const CompilerFlavor buildCompiler;
	REDSTRAIN_REDMOND_API extern const OperatingSystem buildTargetOS;
	REDSTRAIN_REDMOND_API extern const Architecture buildArchitecture;

}}

#endif /* REDSTRAIN_MOD_REDMOND_CONSTANTS_HPP */
