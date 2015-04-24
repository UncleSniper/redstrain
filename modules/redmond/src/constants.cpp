#include "constants.hpp"

namespace redengine {
namespace redmond {

	extern const OperatingSystem buildHostOS =
#if XAKE_HOST_OS == XAKE_OS_LINUX
		OS_LINUX
#elif XAKE_HOST_OS == XAKE_OS_WINDOWS
		OS_WINDOWS
#else /* unknown OS */
		OS_UNKNOWN
#endif /* XAKE_HOST_OS switch */
	;

	extern const CompilerFlavor buildCompiler =
#if XAKE_COMPILER == XAKE_COMPILER_GCC
		COMP_GCC
#elif XAKE_COMPILER == XAKE_COMPILER_MSVC
		COMP_MSVC
#else /* unknown compiler */
		COMP_UNKNOWN
#endif /* XAKE_COMPILER switch */
	;

}}
