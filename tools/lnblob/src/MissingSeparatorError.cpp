#include "MissingSeparatorError.hpp"

MissingSeparatorError::MissingSeparatorError(unsigned lineNumber) : lineNumber(lineNumber) {}

MissingSeparatorError::MissingSeparatorError(const MissingSeparatorError& error)
		: Error(error), lineNumber(error.lineNumber) {}

REDSTRAIN_DEFINE_ERROR(MissingSeparatorError) {
	out << "Missing '=' separator in line " << lineNumber;
}
