#ifndef REDSTRAIN_MOD_UTIL_REF_HPP
#define REDSTRAIN_MOD_UTIL_REF_HPP

#include "Unref.hpp"

/**
 * @file
 * Definition of the @link redengine::util::Ref smart pointer for
 * automatic reference creation/removal notification @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Smart pointer for automatic reference creation/removal notification.
	 * During destruction of such a smart pointer object, the raw
	 * pointer it wraps will be used to notify the object it refers
	 * to that the reference has vanished. See the destructor for
	 * details. Additionally, when a pointer value enters the smart
	 * pointer object, the former object will be notified that a
	 * reference has been created.
	 *
	 * @tparam SubjectT type of objects the pointer value may refer to
	 * @tparconstr
	 * - all constraints applying to Unref
	 * - @c SubjectT must expose a nilary member function <tt>ref</tt>.
	 *
	 * @since 0.1.0
	 */
	template<typename SubjectT>
	class Ref : public Unref<SubjectT> {

	  public:
		/**
		 * Value constructor initializing the wrapped pointer. That is,
		 * the smart pointer will initially refer to the given @c object
		 * (or no object, if @c NULL is given). If @c notify is @c true
		 * and @c object is not <tt>NULL</tt>, the object will be notified
		 * about the creation of this reference by calling its @c ref()
		 * member function.
		 *
		 * @param object initial pointer value
		 * @param notify whether to notify the object about the creation
		 * of this reference
		 *
		 * @since 0.1.0
		 */
		Ref(SubjectT* object = NULL, bool notify = false) : Unref<SubjectT>(object) {
			if(notify && object)
				object->ref();
		}

		/**
		 * Shallow copy-constructor. Initialize the raw pointer value with
		 * that from the given other smart <tt>pointer</tt>. If that pointer
		 * value is not <tt>NULL</tt>, the object it refers to will be
		 * notified about the creation of this reference by calling its
		 * @c ref() member function.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 *
		 * @since 0.1.0
		 */
		Ref(const Unref<SubjectT>& pointer) : Unref<SubjectT>(pointer) {
			if(*pointer)
				pointer->ref();
		}

		using Unref<SubjectT>::operator=;

		/**
		 * Shallow copy-assignment operator. Set the raw pointer value
		 * to that of the given other smart <tt>pointer</tt>. The
		 * cleanup for the current pointer value (i.e. as if the
		 * destructor had been invoked) is performed before this
		 * modification and the reference creation notification
		 * is performed after; therefore this operation is equivalent to
		 * placement-destruction followed by placement-construction
		 * using the @link Ref(const Unref<SubjectT>&) copy-constructor@endlink.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 * @return updated pointer value, i.e. @c *pointer
		 */
		inline SubjectT* operator=(const Unref<SubjectT>& pointer) {
			if(pointer.object)
				pointer.object->ref();
			if(this->object)
				this->object->unref();
			return this->object = pointer.object;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_REF_HPP */
