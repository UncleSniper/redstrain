#ifndef REDSTRAIN_MOD_UTIL_TYPECOMBINATORS_HPP
#define REDSTRAIN_MOD_UTIL_TYPECOMBINATORS_HPP

#include "IntegerBounds.hpp"

namespace redengine {
namespace util {

	template<typename T, typename U>
	class OfTwoIntegerTypes {

	  private:
		enum Width {
			T_IS_WIDER,
			SAME_WIDTH,
			U_IS_WIDER
		};

		template<int, Width, bool, bool>
		struct Impl;

		#define REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(width, tSigned, uSigned, narrower, wider, lower, higher) \
			template<int Dummy> \
			struct Impl<Dummy, width, tSigned, uSigned> { \
				typedef narrower Narrower; \
				typedef wider Wider; \
				typedef lower WithLowerLowerBound; \
				typedef higher WithHigherUpperBound; \
			};

		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(T_IS_WIDER, false, false, U, T, T, T)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(T_IS_WIDER, false, true,  U, T, U, T)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(T_IS_WIDER, true,  false, U, T, T, T)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(T_IS_WIDER, true,  true,  U, T, T, T)

		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(SAME_WIDTH, false, false, T, T, T, T)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(SAME_WIDTH, false, true,  T, T, U, T)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(SAME_WIDTH, true,  false, T, T, T, U)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(SAME_WIDTH, true,  true,  T, T, T, T)

		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(U_IS_WIDER, false, false, T, U, T, U)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(U_IS_WIDER, false, true,  T, U, U, U)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(U_IS_WIDER, true,  false, T, U, T, U)
		REDSTRAIN_UTIL_OFTWOINTEGERTYPES_IMPL(U_IS_WIDER, true,  true,  T, U, U, U)

	  private:
		static const Width WIDTH = sizeof(T) > sizeof(U) ? T_IS_WIDER
				: (sizeof(U) > sizeof(T) ? U_IS_WIDER : SAME_WIDTH);

	  private:
		typedef Impl<0, WIDTH, IntegerBounds<T>::SIGNED, IntegerBounds<U>::SIGNED> I;

	  public:
		typedef typename I::Narrower Narrower;
		typedef typename I::Wider Wider;
		typedef typename I::WithLowerLowerBound WithLowerLowerBound;
		typedef typename I::WithHigherUpperBound WithHigherUpperBound;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_TYPECOMBINATORS_HPP */
