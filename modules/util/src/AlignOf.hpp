#ifndef REDSTRAIN_MOD_UTIL_ALIGNOF_HPP
#define REDSTRAIN_MOD_UTIL_ALIGNOF_HPP

#include "types.hpp"

namespace redengine {
namespace util {

	template<typename T>
	class AlignOf {

	  private:
		struct Dummy {
			char c;
			T t;
		};

	  public:
		static const MemorySize ALIGNMENT = static_cast<MemorySize>(sizeof(Dummy) - sizeof(T));

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_ALIGNOF_HPP */
