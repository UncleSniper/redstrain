#include "UndefinedStringParameterReferencedError.hpp"

namespace redengine {
namespace damnation {

	UndefinedStringParameterReferencedError::UndefinedStringParameterReferencedError(unsigned index)
			: index(index) {}

	UndefinedStringParameterReferencedError::UndefinedStringParameterReferencedError(const
			UndefinedStringParameterReferencedError& error) : Error(error), TParmError(error), index(error.index) {}

	REDSTRAIN_DEFINE_ERROR(UndefinedStringParameterReferencedError) {
		out << "Parameterized string references parameter " << (index + 1u) << ", which was not given";
	}

}}
