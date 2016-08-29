#include "ListModelIndexOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace tk {

	ListModelIndexOutOfBoundsError::ListModelIndexOutOfBoundsError(ModelElementIndex index)
			: IndexOutOfBoundsError(static_cast<MemorySize>(index)), index(index) {}

	ListModelIndexOutOfBoundsError::ListModelIndexOutOfBoundsError(const ListModelIndexOutOfBoundsError& error)
			: Error(error), DamnError(error), IndexOutOfBoundsError(error), index(error.index) {}

	REDSTRAIN_DEFINE_ERROR(ListModelIndexOutOfBoundsError) {
		out << "List model index out of bounds: " << index;
	}

}}}
