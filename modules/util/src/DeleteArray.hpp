#ifndef REDSTRAIN_MOD_UTIL_DELETEARRAY_HPP
#define REDSTRAIN_MOD_UTIL_DELETEARRAY_HPP

#include "Pointer.hpp"

/**
 * @file
 * Definition of the @link redengine::util::DeleteArray smart pointer for
 * automatic array deletion @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Smart pointer for automatic array deletion. During destruction
	 * of such a smart pointer object, the raw pointer it wraps will
	 * be used to destroy and deallocate the array of objects it refers
	 * to. See the destructor for details.
	 *
	 * @b Caution: The C++ @c delete and @c delete[] operators are
	 * @b not interchangeable! Be sure to use Delete for pointers
	 * obtained via @c new and DeleteArray for pointers obtained via
	 * <tt>new[]</tt>!
	 *
	 * @tparam SubjectT type of element objects of the array the pointer
	 * value may refer to
	 *
	 * @since 0.1.0
	 */
	template<typename SubjectT>
	class DeleteArray : public Pointer<SubjectT> {

	  public:
		/**
		 * Value constructor initializing the wrapped pointer. That is,
		 * the smart pointer will initially refer to the given @c array
		 * (or no array, if @c NULL is given). Note that due to the
		 * semantics of #~DeleteArray, this must be either @c NULL or
		 * a pointer returned by an invocation of the C++ @c new[]
		 * operator.
		 *
		 * @param array initial pointer value
		 *
		 * @since 0.1.0
		 */
		DeleteArray(SubjectT* array = NULL) : Pointer<SubjectT>(array) {}

		/**
		 * Shallow copy-constructor. Initialize the raw pointer value with
		 * that from the given other smart <tt>pointer</tt>.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 *
		 * @since 0.1.0
		 */
		DeleteArray(const DeleteArray& pointer) : Pointer<SubjectT>(pointer) {}

		/**
		 * Cleanup destructor for array deletion. If the current pointer
		 * value (@c **this) is @c NULL, no action is taken. Otherwise,
		 * the C++ @c delete[] operator is invoked on that pointer value,
		 * thus destroying all elements of and deallocating the array it
		 * refers to.
		 *
		 * @since 0.1.0
		 */
		~DeleteArray() {
			if(this->object)
				delete[] this->object;
		}

		using Pointer<SubjectT>::operator=;

		/**
		 * Shallow copy-assignment operator. Set the raw pointer value
		 * to that of the given other smart <tt>pointer</tt>. The
		 * cleanup for the current pointer value (i.e. as if the
		 * destructor had been invoked) is performed before this
		 * modification; therefore this operation is equivalent to
		 * placement-destruction followed by placement-construction
		 * using the @link DeleteArray(const DeleteArray&) copy-constructor@endlink.
		 *
		 * @param pointer smart pointer to copy raw pointer value from
		 * @return updated pointer value, i.e. @c *pointer
		 */
		inline SubjectT* operator=(const DeleteArray& pointer) {
			if(this->object)
				delete[] this->object;
			return this->object = pointer.object;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_DELETEARRAY_HPP */
