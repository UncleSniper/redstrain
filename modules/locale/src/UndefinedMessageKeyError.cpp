#include "UndefinedMessageKeyError.hpp"

using std::string;

namespace redengine {
namespace locale {

	UndefinedMessageKeyError::UndefinedMessageKeyError(const string& key, const string& file, unsigned line)
			: MessageKeyError(key, file, line) {}

	UndefinedMessageKeyError::UndefinedMessageKeyError(const UndefinedMessageKeyError& error)
			: Error(error), MessageKeyError(error) {}

	REDSTRAIN_DEFINE_ERROR(UndefinedMessageKeyError) {
		out << "Undefined message key";
		printMessageKeyErrorLocationAndKey(out);
	}

}}
