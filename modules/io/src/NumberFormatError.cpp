#include "NumberFormatError.hpp"

using std::string;

namespace redengine {
namespace io {

	NumberFormatError::NumberFormatError(const string& number) : IllegalArgumentError(number) {}

	NumberFormatError::NumberFormatError(const NumberFormatError& error)
			: Error(error), IllegalArgumentError(error) {}

	REDSTRAIN_DEFINE_ERROR(NumberFormatError) {
		out << "Not a valid number: " << getRawMessage();
	}

}}
