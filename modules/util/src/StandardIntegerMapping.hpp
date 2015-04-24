#ifndef REDSTRAIN_MOD_UTIL_STANDARDINTEGERMAPPING_HPP
#define REDSTRAIN_MOD_UTIL_STANDARDINTEGERMAPPING_HPP

#include <new>
#include <stdint.h>

namespace redengine {
namespace util {

	template<typename IntegerT>
	class StandardIntegerMapping {

	  private:
		template<int, size_t, bool>
		struct Mapping;

#define REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(type, sign) \
	template<int Dummy> \
	struct Mapping<Dummy, sizeof(type), sign> { \
		typedef type StandardType; \
	};

		REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(int8_t, true)
		REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(uint8_t, false)
		REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(int16_t, true)
		REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(uint16_t, false)
		REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(int32_t, true)
		REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(uint32_t, false)
		REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(int64_t, true)
		REDSTRAIN_UTIL_DEFINE_STANDARD_INTEGER_MAPPING(uint64_t, false)

	  private:
		static const bool SIGNED = static_cast<IntegerT>(-1) < static_cast<IntegerT>(0);

	  public:
		typedef typename Mapping<0, sizeof(IntegerT), SIGNED>::StandardType StandardType;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_STANDARDINTEGERMAPPING_HPP */
