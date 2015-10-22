#include "DuplicateMessageKeyError.hpp"

using std::string;

namespace redengine {
namespace locale {

	DuplicateMessageKeyError::DuplicateMessageKeyError(const string& key, const string& file, unsigned line)
			: key(key), file(file), line(line) {}

	DuplicateMessageKeyError::DuplicateMessageKeyError(const DuplicateMessageKeyError& error)
			: LocalizationError(error), key(error.key), file(error.file), line(error.line) {}

	DuplicateMessageKeyError::~DuplicateMessageKeyError() {}

	REDSTRAIN_DEFINE_ERROR(DuplicateMessageKeyError) {
		out << "Duplicate message key";
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

}}
