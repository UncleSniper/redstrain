#define _ISOC99_SOURCE
#include <math.h>

#include "mathwrap.hpp"

namespace redengine {
namespace util {

	#define clamp(cname) case FP_ ## cname: return FPC_ ## cname;
	#define classify \
		switch(fpclassify(value)) { \
			clamp(NAN) \
			clamp(INFINITE) \
			clamp(ZERO) \
			clamp(SUBNORMAL) \
			default: return FPC_NORMAL; \
		}

	REDSTRAIN_UTIL_API FloatingPointClass floatClassify(float value) {
		classify
	}

	REDSTRAIN_UTIL_API FloatingPointClass doubleClassify(double value) {
		classify
	}

}}
