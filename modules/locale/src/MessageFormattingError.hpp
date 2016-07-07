#ifndef REDSTRAIN_MOD_LOCALE_MESSAGEFORMATTINGERROR_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGEFORMATTINGERROR_HPP

#include <redstrain/text/FormattingError.hpp>
#include <redstrain/util/types.hpp>

#include "Locale.hpp"
#include "LocalizationError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API MessageFormattingError : public LocalizationError {

	  private:
		text::FormattingError* formattingError;
		const Locale locale;
		const util::MemorySize messageID;
		const std::string componentName;

	  public:
		MessageFormattingError(text::FormattingError*, const Locale&, util::MemorySize, const std::string&);
		MessageFormattingError(const MessageFormattingError&);
		virtual ~MessageFormattingError();

		inline const text::FormattingError* getFormattingError() const {
			return formattingError;
		}

		inline const Locale& getRequestedLocale() const {
			return locale;
		}

		inline util::MemorySize getRequestedMessageID() const {
			return messageID;
		}

		inline const std::string& getComponentName() const {
			return componentName;
		}

		REDSTRAIN_DECLARE_ERROR(MessageFormattingError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGEFORMATTINGERROR_HPP */
