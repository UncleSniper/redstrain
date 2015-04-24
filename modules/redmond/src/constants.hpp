#ifndef REDSTRAIN_MOD_REDMOND_CONSTANTS_HPP
#define REDSTRAIN_MOD_REDMOND_CONSTANTS_HPP

namespace redengine {
namespace redmond {

	enum OperatingSystem {
		OS_LINUX,
		OS_WINDOWS,
		OS_UNKNOWN
	};

	enum CompilerFlavor {
		COMP_GCC,
		COMP_MSVC,
		COMP_UNKNOWN
	};

	extern const OperatingSystem buildHostOS;
	extern const CompilerFlavor buildCompiler;

}}

#endif /* REDSTRAIN_MOD_REDMOND_CONSTANTS_HPP */
