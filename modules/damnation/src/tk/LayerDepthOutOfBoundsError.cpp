#include "LayerDepthOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace tk {

	LayerDepthOutOfBoundsError::LayerDepthOutOfBoundsError(MemorySize depth) : IndexOutOfBoundsError(depth) {}

	LayerDepthOutOfBoundsError::LayerDepthOutOfBoundsError(const LayerDepthOutOfBoundsError& error)
			: Error(error), DamnError(error), IndexOutOfBoundsError(error) {}

	REDSTRAIN_DEFINE_ERROR(LayerDepthOutOfBoundsError) {
		out << "Layer depth out of bounds: " << getOffendingIndex();
	}

}}}
