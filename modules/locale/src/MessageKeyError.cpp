#include "MessageKeyError.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace locale {

	MessageKeyError::MessageKeyError(const string& key, const string& file, unsigned line)
			: key(key), file(file), line(line) {}

	MessageKeyError::MessageKeyError(const MessageKeyError& error)
			: Error(error), LocalizationError(error), key(error.key), file(error.file), line(error.line) {}

	MessageKeyError::~MessageKeyError() {}

	void MessageKeyError::printMessageKeyErrorLocationAndKey(ostream& out) const {
		if(file.empty()) {
			if(line)
				out << " in line " << line;
		}
		else {
			out << " in '" << file << '\'';
			if(line)
				out << " line " << line;
		}
		out << ": " << key;
	}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(MessageKeyError)

}}
