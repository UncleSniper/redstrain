#ifndef REDSTRAIN_MOD_LOG_TIMEPOINTMESSAGEMEMBERFORMATTER_HPP
#define REDSTRAIN_MOD_LOG_TIMEPOINTMESSAGEMEMBERFORMATTER_HPP

#include <redstrain/text/Transcode.hpp>
#include <redstrain/calendar/FormatMessageCatalog.hpp>

#include "MessageMemberFormatter.hpp"

namespace redengine {
namespace log {

	template<
		calendar::CalendarPrecision Precision,
		calendar::CalendarModulus Modulus,
		typename MessageCatalogT = calendar::FormatMessageCatalog16
	>
	class TimePointMessageMemberFormatter
			: public MessageMemberFormatter<calendar::TimePoint<Precision, Modulus> > {

	  public:
		typedef MessageCatalogT MessageCatalog;

	  private:
		MessageCatalogT& catalog;
		bool manageCatalog;
		const locale::Locale* locale;
		text::String16 formatString;

	  public:
		TimePointMessageMemberFormatter(MessageCatalogT& catalog, bool manageCatalog, const locale::Locale& locale)
				: catalog(catalog), manageCatalog(manageCatalog), locale(&locale),
				formatString(text::Transcode::utf8ToBMP("%4Y-%2m-%2d %2H:%2M:%2s")) {}

		TimePointMessageMemberFormatter(const TimePointMessageMemberFormatter& formatter)
				: MessageMemberFormatter<calendar::TimePoint<Precision, Modulus> >(formatter),
				catalog(formatter.catalog), manageCatalog(false), locale(formatter.locale),
				formatString(formatter.formatString) {}

		virtual ~TimePointMessageMemberFormatter() {
			if(manageCatalog)
				delete &catalog;
		}

		inline MessageCatalogT& getMessageCatalog() {
			return catalog;
		}

		inline const MessageCatalogT& getMessageCatalog() const {
			return catalog;
		}

		inline bool isManageCatalog() const {
			return manageCatalog;
		}

		inline void setManageCatalog(bool manageCatalog) {
			this->manageCatalog = manageCatalog;
		}

		inline const locale::Locale& getLocale() const {
			return *locale;
		}

		void setLocale(const locale::Locale& locale) {
			this->locale = &locale;
		}

		inline const text::String16& getFormatString() const {
			return formatString;
		}

		void setFormatString(const text::String16& formatString) {
			this->formatString = formatString;
		}

		virtual text::String16 formatMember(const calendar::TimePoint<Precision, Modulus>& timestamp) {
			return catalog.formatTime(*locale, formatString, timestamp);
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_TIMEPOINTMESSAGEMEMBERFORMATTER_HPP */
