#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_UTILS_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_UTILS_HPP

#include "api.hpp"

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

	template<typename CollectionT>
	struct UnmangleCollectionPtr {

		CollectionT* ptr;

		UnmangleCollectionPtr(CollectionT* ptr) : ptr(ptr) {}

		~UnmangleCollectionPtr() {
			if(!ptr)
				return;
			typename CollectionT::const_iterator begin(ptr->begin()), end(ptr->end());
			for(; begin != end; ++begin)
				delete *begin;
		}

	};

	REDSTRAIN_REDMOND_API bool shouldDebugUnmanglers();

}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_UTILS_HPP */
