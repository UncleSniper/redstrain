#ifndef REDSTRAIN_MOD_LOCALE_NOMESSAGESFORREQUESTEDLOCALEERROR_HPP
#define REDSTRAIN_MOD_LOCALE_NOMESSAGESFORREQUESTEDLOCALEERROR_HPP

#include "Locale.hpp"
#include "LocalizationError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API NoMessagesForRequestedLocaleError : public LocalizationError {

	  private:
		const Locale locale;

	  public:
		NoMessagesForRequestedLocaleError(const Locale&);
		NoMessagesForRequestedLocaleError(const NoMessagesForRequestedLocaleError&);
		virtual ~NoMessagesForRequestedLocaleError();

		inline const Locale& getRequestedLocale() const {
			return locale;
		}

		REDSTRAIN_DECLARE_ERROR(NoMessagesForRequestedLocaleError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_NOMESSAGESFORREQUESTEDLOCALEERROR_HPP */
