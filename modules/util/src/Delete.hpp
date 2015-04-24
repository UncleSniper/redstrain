#ifndef REDSTRAIN_MOD_UTIL_DELETE_HPP
#define REDSTRAIN_MOD_UTIL_DELETE_HPP

#include "Pointer.hpp"

/**
 * @file
 * Definition of the @link redengine::util::Delete smart pointer for
 * automatic object deletion @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Smart pointer for automatic object deletion. During destruction
	 * of such a smart pointer object, the raw pointer it wraps will
	 * be used to destroy and deallocate the single object it refers to.
	 * See the destructor for details.
	 *
	 * @b Caution: The C++ @c delete and @c delete[] operators are
	 * @b not interchangeable! Be sure to use Delete for pointers
	 * obtained via @c new and DeleteArray for pointers obtained via
	 * <tt>new[]</tt>!
	 *
	 * @tparam SubjectT type of objects the pointer value may refer to
	 *
	 * @since 0.1.0
	 */
	template<typename SubjectT>
	class Delete : public Pointer<SubjectT> {

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
		Delete(SubjectT* object = NULL) : Pointer<SubjectT>(object) {}

		/**
		 * Shallow copy-constructor. Initialize the raw pointer value with
		 * that from the given other smart <tt>pointer</tt>.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 *
		 * @since 0.1.0
		 */
		Delete(const Delete& pointer) : Pointer<SubjectT>(pointer) {}

		/**
		 * Cleanup destructor for object deletion. If the current pointer
		 * value (@c **this) is @c NULL, no action is taken. Otherwise,
		 * the C++ @c delete operator is invoked on that pointer value,
		 * thus destroying and deallocating the object it refers to.
		 *
		 * @since 0.1.0
		 */
		~Delete() {
			if(this->object)
				delete this->object;
		}

		using Pointer<SubjectT>::operator=;

		/**
		 * Shallow copy-assignment operator. Set the raw pointer value
		 * to that of the given other smart <tt>pointer</tt>. The
		 * cleanup for the current pointer value (i.e. as if the
		 * destructor had been invoked) is performed before this
		 * modification; therefore this operation is equivalent to
		 * placement-destruction followed by placement-construction
		 * using the @link Delete(const Delete&) copy-constructor@endlink.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 * @return updated pointer value, i.e. @c *pointer
		 */
		inline SubjectT* operator=(const Delete& pointer) {
			if(this->object)
				delete this->object;
			return this->object = pointer.object;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_DELETE_HPP */
