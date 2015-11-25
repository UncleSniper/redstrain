#include "NumberFormatError.hpp"

using std::string;

namespace redengine {
namespace io {

	NumberFormatError::NumberFormatError(NumberType numberType, const string& rendition)
			: numberType(numberType), rendition(rendition) {}

	NumberFormatError::NumberFormatError(const NumberFormatError& error)
			: Error(error), IllegalArgumentError(error), numberType(error.numberType), rendition(error.rendition) {}

	REDSTRAIN_DEFINE_ERROR(NumberFormatError) {
		out << "Not a valid ";
		switch(numberType) {
			case SIGNED_INTEGER:
				out << "integer";
				break;
			case UNSIGNED_INTEGER:
				out << "unsigned integer";
				break;
			default:
				out << "number";
				break;
		}
		out << ": " << rendition;
	}

}}
