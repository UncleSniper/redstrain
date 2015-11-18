#include "NumberFormatError.hpp"

using std::string;

namespace redengine {
namespace io {

	NumberFormatError::NumberFormatError(const string& rendition) : rendition(rendition) {}

	NumberFormatError::NumberFormatError(const NumberFormatError& error)
			: Error(error), IllegalArgumentError(error), rendition(error.rendition) {}

	REDSTRAIN_DEFINE_ERROR(NumberFormatError) {
		out << "Not a valid number: " << rendition;
	}

}}
