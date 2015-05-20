#include "MappingLookupError.hpp"

namespace redengine {
namespace text {

	MappingLookupError::MappingLookupError() {}

	MappingLookupError::MappingLookupError(const MappingLookupError& error) : TextError(error) {}

	REDSTRAIN_DEFINE_ERROR(MappingLookupError) {
		out << "Character mapping lookup error";
	}

}}
