#include "MessageKeyOutOfBoundsError.hpp"

namespace redengine {
namespace locale {

	MessageKeyOutOfBoundsError::MessageKeyOutOfBoundsError(size_t key) : IndexOutOfBoundsError(key) {}

	MessageKeyOutOfBoundsError::MessageKeyOutOfBoundsError(const MessageKeyOutOfBoundsError& error)
			: Error(error), LocalizationError(error), IndexOutOfBoundsError(error) {}

	REDSTRAIN_DEFINE_ERROR(MessageKeyOutOfBoundsError) {
		out << "Message key out of bounds: " << getOffendingIndex();
	}

}}
