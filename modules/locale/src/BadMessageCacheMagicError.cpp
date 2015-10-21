#include "BadMessageCacheMagicError.hpp"

namespace redengine {
namespace locale {

	BadMessageCacheMagicError::BadMessageCacheMagicError() {}

	BadMessageCacheMagicError::BadMessageCacheMagicError(const BadMessageCacheMagicError& error)
			: Error(error), LocalizationError(error) {}

	REDSTRAIN_DEFINE_ERROR(BadMessageCacheMagicError) {
		out << "Bad message cache magic";
	}

}}
