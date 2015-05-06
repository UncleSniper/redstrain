#include "AnnouncedSizeMismatchError.hpp"

namespace redengine {
namespace protostr {

	AnnouncedSizeMismatchError::AnnouncedSizeMismatchError() {}

	AnnouncedSizeMismatchError::AnnouncedSizeMismatchError(const AnnouncedSizeMismatchError& error)
			: Error(error), StateError(error), ProtocolError(error) {}

	REDSTRAIN_DEFINE_ERROR(AnnouncedSizeMismatchError) {
		out << "Actual element size does not match announced size";
	}

}}
