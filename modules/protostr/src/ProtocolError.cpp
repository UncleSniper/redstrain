#include "ProtocolError.hpp"

namespace redengine {
namespace protostr {

	ProtocolError::ProtocolError() {}

	ProtocolError::ProtocolError(const ProtocolError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(ProtocolError)

}}
