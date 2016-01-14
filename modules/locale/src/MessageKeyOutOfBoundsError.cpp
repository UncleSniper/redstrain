#include "MessageKeyOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace locale {

	MessageKeyOutOfBoundsError::MessageKeyOutOfBoundsError(MemorySize key) : IndexOutOfBoundsError(key) {}

	MessageKeyOutOfBoundsError::MessageKeyOutOfBoundsError(const MessageKeyOutOfBoundsError& error)
			: Error(error), LocalizationError(error), IndexOutOfBoundsError(error) {}

	REDSTRAIN_DEFINE_ERROR(MessageKeyOutOfBoundsError) {
		out << "Message key out of bounds: " << getOffendingIndex();
	}

}}
