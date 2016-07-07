#ifndef REDSTRAIN_MOD_LOCALE_NOMESSAGESFORREQUESTEDLOCALEERROR_HPP
#define REDSTRAIN_MOD_LOCALE_NOMESSAGESFORREQUESTEDLOCALEERROR_HPP

#include "Locale.hpp"
#include "LocalizationError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API NoMessagesForRequestedLocaleError : public LocalizationError {

	  private:
		const Locale locale;
		const std::string componentName;

	  public:
		NoMessagesForRequestedLocaleError(const Locale&, const std::string&);
		NoMessagesForRequestedLocaleError(const NoMessagesForRequestedLocaleError&);
		virtual ~NoMessagesForRequestedLocaleError();

		inline const Locale& getRequestedLocale() const {
			return locale;
		}

		inline const std::string& getComponentName() const {
			return componentName;
		}

		REDSTRAIN_DECLARE_ERROR(NoMessagesForRequestedLocaleError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_NOMESSAGESFORREQUESTEDLOCALEERROR_HPP */
