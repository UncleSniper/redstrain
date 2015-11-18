#include "ListIndexOutOfBoundsError.hpp"

namespace redengine {
namespace error {

	ListIndexOutOfBoundsError::ListIndexOutOfBoundsError(size_t index) : IndexOutOfBoundsError(index) {}

	ListIndexOutOfBoundsError::ListIndexOutOfBoundsError(const ListIndexOutOfBoundsError& error)
			: IndexOutOfBoundsError(error) {}

	REDSTRAIN_DEFINE_ERROR(ListIndexOutOfBoundsError) {
		out << "List index out of bounds: " << getOffendingIndex();
	}

}}
