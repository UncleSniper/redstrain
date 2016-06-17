#include <cstdlib>

#include "api.hpp"

namespace redengine {
namespace redmond {

	REDSTRAIN_REDMOND_API bool shouldDebugUnmanglers() {
		enum Result {
			NO,
			YES,
			UNKNOWN
		};
		static Result result = UNKNOWN;
		if(result == UNKNOWN)
			result = getenv("REDSTRAIN_DEBUG_UNMANGLE") ? YES : NO;
		return !!result;
	}

}}
