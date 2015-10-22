#include "MissingMessageKeyError.hpp"

using std::string;

namespace redengine {
namespace locale {

	MissingMessageKeyError::MissingMessageKeyError(const string& key, const string& file)
			: MessageKeyError(key, file, 0u) {}

	MissingMessageKeyError::MissingMessageKeyError(const MissingMessageKeyError& error)
			: Error(error), MessageKeyError(error) {}

	REDSTRAIN_DEFINE_ERROR(MissingMessageKeyError) {
		out << "Missing message key";
		printMessageKeyErrorLocationAndKey(out);
	}

}}
