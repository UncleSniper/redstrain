#ifndef REDSTRAIN_MOD_UTIL_ALIGNOF_HPP
#define REDSTRAIN_MOD_UTIL_ALIGNOF_HPP

#include <cstddef>

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
		static const size_t ALIGNMENT = sizeof(Dummy) - sizeof(T);

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_ALIGNOF_HPP */
