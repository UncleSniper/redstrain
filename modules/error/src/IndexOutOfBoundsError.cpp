#include "IndexOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace error {

	IndexOutOfBoundsError::IndexOutOfBoundsError(MemorySize index) : index(index) {}

	IndexOutOfBoundsError::IndexOutOfBoundsError(const IndexOutOfBoundsError& error)
			: Error(error), IllegalArgumentError(error), index(error.index) {}

	REDSTRAIN_DEFINE_ERROR(IndexOutOfBoundsError) {
		out << "Index out of bounds: " << index;
	}

}}
