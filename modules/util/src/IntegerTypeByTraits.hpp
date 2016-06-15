#ifndef REDSTRAIN_MOD_UTIL_INTEGERTYPEBYTRAITS_HPP
#define REDSTRAIN_MOD_UTIL_INTEGERTYPEBYTRAITS_HPP

#include <cstddef>
#include <stdint.h>

namespace redengine {
namespace util {

	template<size_t Size, bool Signed>
	class IntegerTypeByTraits {

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

	  public:
		typedef typename Mapping<0, Size, Signed>::StandardType StandardType;

	};

	typedef IntegerTypeByTraits<sizeof(void*), false>::StandardType IntegerPointer;

	typedef IntegerTypeByTraits<sizeof(float), false>::StandardType FloatBits;
	typedef IntegerTypeByTraits<sizeof(double), false>::StandardType DoubleBits;

}}

#endif /* REDSTRAIN_MOD_UTIL_INTEGERTYPEBYTRAITS_HPP */
