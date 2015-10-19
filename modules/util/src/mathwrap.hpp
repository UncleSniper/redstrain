#ifndef REDSTRAIN_MOD_UTIL_MATHWRAP_HPP
#define REDSTRAIN_MOD_UTIL_MATHWRAP_HPP

#include "api.hpp"

namespace redengine {
namespace util {

	enum REDSTRAIN_UTIL_API FloatingPointClass {
		FPC_NAN,
		FPC_INFINITE,
		FPC_ZERO,
		FPC_SUBNORMAL,
		FPC_NORMAL
	};

	REDSTRAIN_UTIL_API FloatingPointClass floatClassify(float);
	REDSTRAIN_UTIL_API FloatingPointClass doubleClassify(double);

	template<typename Float>
	class FloatingPointClassify;

	template<>
	class FloatingPointClassify<float> {

	  public:
		static inline FloatingPointClass classify(float value) {
			return floatClassify(value);
		}

	};

	template<>
	class FloatingPointClassify<double> {

	  public:
		static inline FloatingPointClass classify(double value) {
			return doubleClassify(value);
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_MATHWRAP_HPP */
