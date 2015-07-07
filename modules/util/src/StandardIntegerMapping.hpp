#ifndef REDSTRAIN_MOD_UTIL_STANDARDINTEGERMAPPING_HPP
#define REDSTRAIN_MOD_UTIL_STANDARDINTEGERMAPPING_HPP

#include "IntegerTypeByTraits.hpp"

namespace redengine {
namespace util {

	template<typename IntegerT>
	class StandardIntegerMapping {

	  private:
		static const bool SIGNED = static_cast<IntegerT>(-1) < static_cast<IntegerT>(0);

	  public:
		typedef typename IntegerTypeByTraits<sizeof(IntegerT), SIGNED>::StandardType StandardType;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_STANDARDINTEGERMAPPING_HPP */
