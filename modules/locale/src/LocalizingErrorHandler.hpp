#ifndef REDSTRAIN_MOD_LOCALE_LOCALIZINGERRORHANDLER_HPP
#define REDSTRAIN_MOD_LOCALE_LOCALIZINGERRORHANDLER_HPP

#include <redstrain/text/ErrorHandlerBase16.hpp>

#include "StackTraceMessageSource.hpp"
#include "api.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API LocalizingErrorHandler : public virtual text::ErrorHandlerBase16 {

	  private:
		StackTraceMessageSource<text::Char16>& messageSource;
		const Locale* desiredLocale;

	  protected:
		virtual void writeErrorType(const char*);
		virtual void writeErrorMessage(const error::Error&);
		virtual void failedToLocalizeErrorMessage(const error::Error&);

	  public:
		LocalizingErrorHandler();
		LocalizingErrorHandler(const Locale&);
		LocalizingErrorHandler(StackTraceMessageSource<text::Char16>&);
		LocalizingErrorHandler(StackTraceMessageSource<text::Char16>&, const Locale&);
		LocalizingErrorHandler(const LocalizingErrorHandler&);

		inline StackTraceMessageSource<text::Char16>& getMessageSource() {
			return messageSource;
		}

		inline const StackTraceMessageSource<text::Char16>& getMessageSource() const {
			return messageSource;
		}

		inline const Locale& getDesiredLocale() const {
			return *desiredLocale;
		}

		void setDesiredLocale(const Locale&);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_LOCALIZINGERRORHANDLER_HPP */
