#include <ctime>
#include <cstdlib>

#include "IntegerLog.hpp"
#include "IntegerBits.hpp"
#include "api.hpp"

namespace redengine {
namespace util {

	REDSTRAIN_UTIL_API void ensureRandomSeeded() {
		static bool seeded = false;
		if(!seeded) {
			srandom(static_cast<unsigned>(time(NULL)));
			seeded = true;
		}
	}

}}
