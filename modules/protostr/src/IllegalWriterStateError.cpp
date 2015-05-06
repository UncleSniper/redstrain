#include "IllegalWriterStateError.hpp"

namespace redengine {
namespace protostr {

	IllegalWriterStateError::IllegalWriterStateError() {}

	IllegalWriterStateError::IllegalWriterStateError(const IllegalWriterStateError& error)
			: Error(error), StateError(error), ProtocolError(error) {}

	REDSTRAIN_DEFINE_ERROR(IllegalWriterStateError) {
		out << "Illegal protocol writer state: Out of sequence composite call";
	}

}}
