#include "constants.hpp"
#include "environment.hpp"

namespace redengine {
namespace redmond {

	extern const OperatingSystem buildHostOS =
#if defined(REDSTRAIN_ENV_OS_LINUX)
		OS_LINUX
#elif defined(REDSTRAIN_ENV_OS_WINDOWS)
		OS_WINDOWS
#else /* unknown OS */
		OS_UNKNOWN
#endif /* host OS switch */
	;

	extern const CompilerFlavor buildCompiler =
#if defined(REDSTRAIN_ENV_COMPILER_GNU)
		COMP_GCC
#elif defined(REDSTRAIN_ENV_COMPILER_MSVC)
		COMP_MSVC
#else /* unknown compiler */
		COMP_UNKNOWN
#endif /* build compiler switch */
	;

	//TODO: support cross-compilation
	extern const OperatingSystem buildTargetOS = buildHostOS;

}}
