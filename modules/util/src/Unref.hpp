#ifndef REDSTRAIN_MOD_UTIL_UNREF_HPP
#define REDSTRAIN_MOD_UTIL_UNREF_HPP

#include "Pointer.hpp"

/**
 * @file
 * Definition of the @link redengine::util::Unref smart pointer for
 * automatic reference removal notification @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Smart pointer for automatic reference removal notification.
	 * During destruction of such a smart pointer object, the raw
	 * pointer it wraps will be used to notify the object it refers
	 * to that the reference has vanished. See the destructor for
	 * details.
	 *
	 * @tparam SubjectT type of objects the pointer value may refer to
	 * @tparconstr
	 * - @c SubjectT must expose a nilary member function <tt>unref</tt>.
	 *
	 * @since 0.1.0
	 */
	template<typename SubjectT>
	class Unref : public Pointer<SubjectT> {

	  public:
		/**
		 * Value constructor initializing the wrapped pointer. That is,
		 * the smart pointer will initially refer to the given @c object
		 * (or no object, if @c NULL is given).
		 *
		 * @param object initial pointer value
		 *
		 * @since 0.1.0
		 */
		Unref(SubjectT* object = NULL) : Pointer<SubjectT>(object) {}

		/**
		 * Shallow copy-constructor. Initialize the raw pointer value with
		 * that from the given other smart <tt>pointer</tt>.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 *
		 * @since 0.1.0
		 */
		Unref(const Unref& pointer) : Pointer<SubjectT>(pointer) {}

		/**
		 * Cleanup destructor for reference removal notification. If
		 * the current pointer value (@c **this) is @c NULL, no action
		 * is taken. Otherwise, the pointer value is used to call the
		 * @c unref() member function on the object it refers to.
		 * Typically, that function would decrement the reference count
		 * of the object and delete it if the count reaches zero. An
		 * easy way to achieve this is to have @c SubjectT inherit from
		 * ReferenceCounted.
		 *
		 * @since 0.1.0
		 */
		~Unref() {
			if(this->object)
				this->object->unref();
		}

		using Pointer<SubjectT>::operator=;

		/**
		 * Shallow copy-assignment operator. Set the raw pointer value
		 * to that of the given other smart <tt>pointer</tt>. The
		 * cleanup for the current pointer value (i.e. as if the
		 * destructor had been invoked) is performed before this
		 * modification; therefore this operation is equivalent to
		 * placement-destruction followed by placement-construction
		 * using the @link Unref(const Unref&) copy-constructor@endlink.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 * @return updated pointer value, i.e. @c *pointer
		 */
		inline SubjectT* operator=(const Unref& pointer) {
			if(this->object)
				this->object->unref();
			return this->object = pointer.object;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_UNREF_HPP */
