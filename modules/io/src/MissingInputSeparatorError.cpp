#include "MissingInputSeparatorError.hpp"

using std::string;

namespace redengine {
namespace io {

	MissingInputSeparatorError::MissingInputSeparatorError(const string& separator, const string& file,
			unsigned lineNumber) : separator(separator), file(file), lineNumber(lineNumber) {}

	MissingInputSeparatorError::MissingInputSeparatorError(const MissingInputSeparatorError& error)
			: Error(error), separator(error.separator), file(error.file), lineNumber(error.lineNumber) {}

	REDSTRAIN_DEFINE_ERROR(MissingInputSeparatorError) {
		out << "Missing '" << separator << "' separator";
		if(!file.empty()) {
			out << " in '" << file << '\'';
			if(lineNumber)
				out << " line " << lineNumber;
		}
		else {
			if(lineNumber)
				out << " in line " << lineNumber;
		}
	}

}}
