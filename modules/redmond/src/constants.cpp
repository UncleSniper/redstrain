#include "constants.hpp"
#include "environment.hpp"

namespace redengine {
namespace redmond {

	extern const OperatingSystem buildHostOS =
#if defined(XAKE_HOST_OS) && defined(XAKE_OS_LINUX) && XAKE_HOST_OS == XAKE_OS_LINUX
		OS_LINUX
#elif defined(XAKE_HOST_OS) && defined(XAKE_OS_WINDOWS) && XAKE_HOST_OS == XAKE_OS_WINDOWS
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

	extern const OperatingSystem buildTargetOS =
#if defined(REDSTRAIN_ENV_OS_LINUX)
		OS_LINUX
#elif defined(REDSTRAIN_ENV_OS_WINDOWS)
		OS_WINDOWS
#else /* unknown OS */
		OS_UNKNOWN
#endif /* target OS switch */
	;

	extern const Architecture buildArchitecture =
#if defined(REDSTRAIN_ENV_ARCH_I686)
		ARCH_I686
#elif defined(REDSTRAIN_ENV_ARCH_AMD64)
		ARCH_X86_64
#else /* unknown architecture */
		ARCH_UNKNOWN
#endif /* architecture switch */
	;

}}
