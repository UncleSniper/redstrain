#include "IllegalReaderStateError.hpp"

namespace redengine {
namespace protostr {

	IllegalReaderStateError::IllegalReaderStateError() {}

	IllegalReaderStateError::IllegalReaderStateError(const IllegalReaderStateError& error)
			: Error(error), StateError(error), ProtocolError(error) {}

	REDSTRAIN_DEFINE_ERROR(IllegalReaderStateError) {
		out << "Illegal protocol reader state: Out of sequence composite call";
	}

}}
