#include "environment.hpp"
#include "unmangle-gcc.hpp"

using std::string;
using redengine::redmond::unmangle::CPPSymbol;

namespace redengine {
namespace redmond {

#ifdef REDSTRAIN_ENV_COMPILER_GNU

	REDSTRAIN_REDMOND_API CPPSymbol* unmangleCPPSymbol_native(const string& symbol) {
		return unmangleCPPSymbol_GCC3(symbol);
	}

#else /* unknown compiler */

	REDSTRAIN_REDMOND_API CPPSymbol* unmangleCPPSymbol_native(const string&) {
		return NULL;
	}

#endif /* compiler switch */

}}
