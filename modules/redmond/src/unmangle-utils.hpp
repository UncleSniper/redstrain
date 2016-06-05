#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_UTILS_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_UTILS_HPP

namespace redengine {
namespace redmond {

	template<typename T>
	struct UnmanglePtr {

		T* ptr;

		UnmanglePtr(T* ptr) : ptr(ptr) {}

		~UnmanglePtr() {
			if(ptr)
				delete ptr;
		}

	};

}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_UTILS_HPP */
