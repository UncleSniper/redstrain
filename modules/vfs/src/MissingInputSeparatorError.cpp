#include "MissingInputSeparatorError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	MissingInputSeparatorError::MissingInputSeparatorError(const string& separator, unsigned lineNumber)
			: separator(separator), lineNumber(lineNumber) {}

	MissingInputSeparatorError::MissingInputSeparatorError(const MissingInputSeparatorError& error)
			: Error(error), separator(error.separator), lineNumber(error.lineNumber) {}

	REDSTRAIN_DEFINE_ERROR(MissingInputSeparatorError) {
		out << "Missing '" << separator << "' separator in line " << lineNumber;
	}

}}
