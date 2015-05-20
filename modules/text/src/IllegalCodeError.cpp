#include "IllegalCodeError.hpp"

namespace redengine {
namespace text {

	IllegalCodeError::IllegalCodeError() {}

	IllegalCodeError::IllegalCodeError(const IllegalCodeError& error) : RenditionError(error) {}

	REDSTRAIN_DEFINE_ERROR(IllegalCodeError) {
		out << "Illegal byte sequence in text encoding";
	}

}}
