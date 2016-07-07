#include "NoMessagesForRequestedLocaleError.hpp"

using std::string;

namespace redengine {
namespace locale {

	NoMessagesForRequestedLocaleError::NoMessagesForRequestedLocaleError(const Locale& locale,
			const string& componentName) : locale(locale), componentName(componentName) {}

	NoMessagesForRequestedLocaleError::NoMessagesForRequestedLocaleError(const
			NoMessagesForRequestedLocaleError& error)
			: Error(error), LocalizationError(error), locale(error.locale), componentName(error.componentName) {}

	NoMessagesForRequestedLocaleError::~NoMessagesForRequestedLocaleError() {}

	REDSTRAIN_DEFINE_ERROR(NoMessagesForRequestedLocaleError) {
		out << "Could not load messages for locale '" << locale.getName() << '\'';
		if(!componentName.empty())
			out << " in component '" << componentName << '\'';
	}

}}
