#ifndef REDSTRAIN_MOD_UTIL_MAXALIGN_HPP
#define REDSTRAIN_MOD_UTIL_MAXALIGN_HPP

#include "AlignOf.hpp"
#include "api.hpp"

namespace redengine {
namespace util {

	class REDSTRAIN_UTIL_API MaxAlign {

	  private:
		struct Dummy {};

		union Primitives {
			char _char;
			short _short;
			int _int;
			long _long;
			long long _longlong;
			float _float;
			double _double;
			long double _longdouble;
			void* _pointer;
			int (*_function)(int);
			int (Dummy::*_method)(int);
		};

	  public:
		static const size_t MAX_ALIGNMENT = AlignOf<Primitives>::ALIGNMENT;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_MAXALIGN_HPP */
