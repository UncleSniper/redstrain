#include "ProtocolError.hpp"
#include "ProtocolStreamError.hpp"

namespace redengine {
namespace protostr {

	ProtocolStreamError::ProtocolStreamError(ProtocolError* cause) : cause(cause) {}

	ProtocolStreamError::ProtocolStreamError(const ProtocolStreamError& error)
			: Error(error), IOError(error), cause(error.cause) {}

	ProtocolError* ProtocolStreamError::getCause() const {
		return *cause;
	}

	REDSTRAIN_DEFINE_ERROR(ProtocolStreamError) {
		if(*cause)
			cause->printMessage(out);
		else
			out << "Protocol error in protocol stream";
	}

}}
