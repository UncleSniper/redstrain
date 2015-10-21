#include "NoMessagesForRequestedLocaleError.hpp"

namespace redengine {
namespace locale {

	NoMessagesForRequestedLocaleError::NoMessagesForRequestedLocaleError(const Locale& locale) : locale(locale) {}

	NoMessagesForRequestedLocaleError::NoMessagesForRequestedLocaleError(const
			NoMessagesForRequestedLocaleError& error)
			: Error(error), LocalizationError(error), locale(error.locale) {}

	NoMessagesForRequestedLocaleError::~NoMessagesForRequestedLocaleError() {}

	REDSTRAIN_DEFINE_ERROR(NoMessagesForRequestedLocaleError) {
		out << "Could not load messages for locale '" << locale.getName() << '\'';
	}

}}
