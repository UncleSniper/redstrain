#ifndef REDSTRAIN_MOD_UTIL_POINTER_HPP
#define REDSTRAIN_MOD_UTIL_POINTER_HPP

#include <string>

/**
 * @file
 * Definition of the @link redengine::util::Pointer common base for
 * classes wrapping pointers @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Common base for classes wrapping pointers. This class provides a single
	 * data member, which is a pointer value to an object of type
	 * <tt>SubjectT</tt>. Derived classes may impose some manner of "smart
	 * pointer" semantics on this value. Consequently, the typical derived
	 * class will perform a cleanup action in its destructor, such that the
	 * client can be sure this action will be performed when the pointer falls
	 * out of scope, even if an exception is thrown from inside said scope.
	 *
	 * @tparam SubjectT type of objects the pointer value may refer to
	 *
	 * @since 0.1.0
	 */
	template<typename SubjectT>
	class Pointer {

	  public:
		/**
		 * Type of objects the pointer may refer to. Present so that the type
		 * parameter can be extracted from a template instance.
		 *
		 * @since 0.1.0
		 */
		typedef SubjectT Subject;

	  protected:
		/**
		 * Wrapped pointer value. That is, this member points to the object
		 * whose lifecycle is managed by this smart pointer. Note that this
		 * may be @c NULL if the client sets it so.
		 *
		 * @since 0.1.0
		 */
		SubjectT* object;

	  public:
		/**
		 * Base contructor initializing the wrapped pointer. This will set
		 * the @link #object data member @endlink of the same name to the
		 * value of @c object as a required initialization. Derived classes
		 * may hide this requirement from the client; in fact, they should,
		 * by convention, expose a constructor with the same argument, but
		 * with @c NULL as the default.
		 *
		 * @param object the object this smart pointer refers to
		 *
		 * @since 0.1.0
		 */
		Pointer(SubjectT* object) : object(object) {}

		/**
		 * Shallow copy-constructor. Initialize the raw pointer value with
		 * that from the given other smart <tt>pointer</tt>.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 *
		 * @since 0.1.0
		 */
		Pointer(const Pointer& pointer) : object(pointer.object) {}

		/**
		 * Unwrap this smart pointer. That is, dereferencing the smart pointer
		 * object yields the raw pointer it wraps, pointing to the object it
		 * refers to.
		 *
		 * @return the current pointer value
		 *
		 * @since 0.1.0
		 */
		inline SubjectT* operator*() const {
			return object;
		}

		/**
		 * Unwrap this smart pointer. That is, dereferencing the smart pointer
		 * object yields the raw pointer it wraps, pointing to the object it
		 * refers to. If @c SubjectT is an aggregate type, the @c ->
		 * operator may thus be used to obtain a reference to one of the
		 * members of the object.
		 *
		 * @return the current pointer value
		 *
		 * @since 0.1.0
		 */
		inline SubjectT* operator->() const {
			return object;
		}

		/**
		 * Modify wrapped pointer value. That is, set the #object member to
		 * the value of <tt>newObject</tt>. As the pointer value prior to this
		 * modification is returned, this function is commonly used to "move"
		 * the object out of the smart pointer; i.e. the typical use case is:
		 * @code
		   SomeDerived<SomeSubject> ptr(new SomeSubject);
		   // work with *ptr
		   return ptr.set();
		 * @endcode
		 *
		 * @param newObject pointer value to set this smart pointer to
		 * @return previous pointer value
		 *
		 * @since 0.1.0
		 */
		SubjectT* set(SubjectT* newObject = NULL) {
			SubjectT* oldObject = object;
			object = newObject;
			return oldObject;
		}

		/**
		 * Modify wrapped pointer value. That is, set the #object member to
		 * the value of @c newObject. The assignment yields the assigned
		 * value, making it different from #set with respect to its return
		 * value.
		 * @param newObject pointer value to set this smart pointer to
		 * @return updated pointer value, i.e. @c newObject
		 *
		 * @since 0.1.0
		 */
		inline SubjectT* operator=(SubjectT* newObject) {
			return object = newObject;
		}

		inline SubjectT*& pointerLocation() {
			return object;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_POINTER_HPP */
