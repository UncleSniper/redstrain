#ifndef REDSTRAIN_MOD_UTIL_VLARRAY_HPP
#define REDSTRAIN_MOD_UTIL_VLARRAY_HPP

#ifdef __GNUC__

#include "DestroyArray.hpp"
#define REDSTRAIN_VLARRAY_DEFINE(elementType, name, elementCount) \
	elementType* name = static_cast<elementType*>(__builtin_alloca(static_cast<size_t>(elementCount) \
			* sizeof(elementType))); \
	::redengine::util::DestroyArray<elementType> _ ## name ## _destroyer(name); \
	_ ## name ## _destroyer.extend(static_cast< ::redengine::util::MemorySize>(elementCount));
#define REDSTRAIN_VLARRAY_ACCESS(name) name

#else /* unknown compiler */

#include "DeleteArray.hpp"
#define REDSTRAIN_VLARRAY_DEFINE(elementType, name, elementCount) \
	::redengine::util::DeleteArray<elementType> name(new elementType[static_cast<size_t>(elementCount)]);
#define REDSTRAIN_VLARRAY_ACCESS(name) (*name)

#endif /* compiler differences */

#endif /* REDSTRAIN_MOD_UTIL_VLARRAY_HPP */
