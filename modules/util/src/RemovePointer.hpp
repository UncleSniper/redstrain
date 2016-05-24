#ifndef REDSTRAIN_MOD_UTIL_REMOVEPOINTER_HPP
#define REDSTRAIN_MOD_UTIL_REMOVEPOINTER_HPP

namespace redengine {
namespace util {

	template<typename T>
	class RemovePointer {

	  public:
		typedef T Component;
		typedef T Leaf;

	  public:
		static const unsigned POINTER_LEVELS = 0u;

	};

	template<typename T>
	class RemovePointer<T*> {

	  public:
		typedef T Component;
		typedef typename RemovePointer<T>::Leaf Leaf;

	  public:
		static const unsigned POINTER_LEVELS = RemovePointer<T>::POINTER_LEVELS + 1u;

	};

	template<typename T>
	class RemovePointer<T *const> {

	  public:
		typedef T Component;
		typedef typename RemovePointer<T>::Leaf Leaf;

	  public:
		static const unsigned POINTER_LEVELS = RemovePointer<T>::POINTER_LEVELS + 1u;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_REMOVEPOINTER_HPP */
