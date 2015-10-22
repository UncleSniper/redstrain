#include "DuplicateMessageKeyError.hpp"

using std::string;

namespace redengine {
namespace locale {

	DuplicateMessageKeyError::DuplicateMessageKeyError(const string& key, const string& file, unsigned line)
			: MessageKeyError(key, file, line) {}

	DuplicateMessageKeyError::DuplicateMessageKeyError(const DuplicateMessageKeyError& error)
			: Error(error), MessageKeyError(error) {}

	REDSTRAIN_DEFINE_ERROR(DuplicateMessageKeyError) {
		out << "Duplicate message key";
		printMessageKeyErrorLocationAndKey(out);
	}

}}
