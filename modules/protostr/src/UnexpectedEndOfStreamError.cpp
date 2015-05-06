#include "UnexpectedEndOfStreamError.hpp"

namespace redengine {
namespace protostr {

	UnexpectedEndOfStreamError::UnexpectedEndOfStreamError() {}

	UnexpectedEndOfStreamError::UnexpectedEndOfStreamError(const UnexpectedEndOfStreamError& error)
			: Error(error), ProtocolError(error) {}

	REDSTRAIN_DEFINE_ERROR(UnexpectedEndOfStreamError) {
		out << "Unexpected end of protocol input";
	}

}}
