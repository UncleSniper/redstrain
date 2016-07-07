#ifndef REDSTRAIN_MOD_CALENDAR_FORMATMESSAGECATALOG_HPP
#define REDSTRAIN_MOD_CALENDAR_FORMATMESSAGECATALOG_HPP

#include <redstrain/locale/FormattedMessageCatalog.hpp>
#include <redstrain/platform/PropertyMutexLockingPolicy.hpp>
#include <redstrain/text/types.hpp>

#include "TimeFormatter.hpp"
#include "FormatMessageKey.hpp"
#include "LocalizedTimeFormat.hpp"

namespace redengine {
namespace calendar {

	template<
		typename CharT,
		typename LockingPolicyT,
		typename NumericRenditionT
			= typename locale::FormattedMessageCatalog<
				CharT,
				FormatMessageKey,
				LockingPolicyT
			>::Formatter::NumericRendition,
		typename IntegerFormatterT
			= typename locale::FormattedMessageCatalog<
				CharT,
				FormatMessageKey,
				LockingPolicyT
			>::Formatter::IntegerFormatter,
		typename FloatFormatterT
			= typename locale::FormattedMessageCatalog<
				CharT,
				FormatMessageKey,
				LockingPolicyT
			>::Formatter::FloatFormatter,
		typename TimeNumericRenditionT = typename TimeFormatter<CharT>::NumericRendition,
		typename TimeIntegerFormatterT = typename TimeFormatter<CharT>::IntegerFormatter,
		typename TimeFormatRenditionT = typename TimeFormatter<CharT>::FormatRendition
	>
	class FormatMessageCatalog : public locale::FormattedMessageCatalog<
		CharT,
		FormatMessageKey,
		LockingPolicyT,
		NumericRenditionT,
		IntegerFormatterT,
		FloatFormatterT
	> {

	  public:
		typedef std::basic_string<CharT> String;

	  public:
		class FormatMessageCatalogTimeStringEmitter : public TimeStringEmitter<CharT> {

		  private:
			FormatMessageCatalog& catalog;
			const locale::Locale& locale;

		  public:
			FormatMessageCatalogTimeStringEmitter(FormatMessageCatalog& catalog, const locale::Locale& locale)
					: catalog(catalog), locale(locale) {}

			FormatMessageCatalogTimeStringEmitter(const FormatMessageCatalogTimeStringEmitter& emitter)
					: TimeStringEmitter<CharT>(emitter), catalog(emitter.catalog), locale(emitter.locale) {}

			virtual String emitString(TimeStringOption option) {
				return catalog.getTimeOptionString(locale, option);
			}

		};

	  public:
		FormatMessageCatalog() {}

		FormatMessageCatalog(typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: locale::FormattedMessageCatalog<CharT, FormatMessageKey, LockingPolicyT, NumericRenditionT,
				IntegerFormatterT, FloatFormatterT>(lockInitializer) {}

		FormatMessageCatalog(const FormatMessageCatalog& catalog)
				: locale::FormattedMessageCatalog<CharT, FormatMessageKey, LockingPolicyT, NumericRenditionT,
				IntegerFormatterT, FloatFormatterT>(catalog) {}

		String getTimeOptionString(const locale::Locale& locale, TimeStringOption option) {
			switch(option) {
				case TSO_MON:
					return this->formatMessage(locale, FMK_TSO_MON);
				case TSO_TUE:
					return this->formatMessage(locale, FMK_TSO_TUE);
				case TSO_WED:
					return this->formatMessage(locale, FMK_TSO_WED);
				case TSO_THU:
					return this->formatMessage(locale, FMK_TSO_THU);
				case TSO_FRI:
					return this->formatMessage(locale, FMK_TSO_FRI);
				case TSO_SAT:
					return this->formatMessage(locale, FMK_TSO_SAT);
				case TSO_SUN:
					return this->formatMessage(locale, FMK_TSO_SUN);
				case TSO_MONDAY:
					return this->formatMessage(locale, FMK_TSO_MONDAY);
				case TSO_TUESDAY:
					return this->formatMessage(locale, FMK_TSO_TUESDAY);
				case TSO_WEDNESDAY:
					return this->formatMessage(locale, FMK_TSO_WEDNESDAY);
				case TSO_THURSDAY:
					return this->formatMessage(locale, FMK_TSO_THURSDAY);
				case TSO_FRIDAY:
					return this->formatMessage(locale, FMK_TSO_FRIDAY);
				case TSO_SATURDAY:
					return this->formatMessage(locale, FMK_TSO_SATURDAY);
				case TSO_SUNDAY:
					return this->formatMessage(locale, FMK_TSO_SUNDAY);
				case TSO_JAN:
					return this->formatMessage(locale, FMK_TSO_JAN);
				case TSO_FEB:
					return this->formatMessage(locale, FMK_TSO_FEB);
				case TSO_MAR:
					return this->formatMessage(locale, FMK_TSO_MAR);
				case TSO_APR:
					return this->formatMessage(locale, FMK_TSO_APR);
				case TSO_MAY_S:
					return this->formatMessage(locale, FMK_TSO_MAY_S);
				case TSO_JUN:
					return this->formatMessage(locale, FMK_TSO_JUN);
				case TSO_JUL:
					return this->formatMessage(locale, FMK_TSO_JUL);
				case TSO_AUG:
					return this->formatMessage(locale, FMK_TSO_AUG);
				case TSO_SEP:
					return this->formatMessage(locale, FMK_TSO_SEP);
				case TSO_OCT:
					return this->formatMessage(locale, FMK_TSO_OCT);
				case TSO_NOV:
					return this->formatMessage(locale, FMK_TSO_NOV);
				case TSO_DEC:
					return this->formatMessage(locale, FMK_TSO_DEC);
				case TSO_JANUARY:
					return this->formatMessage(locale, FMK_TSO_JANUARY);
				case TSO_FEBRUARY:
					return this->formatMessage(locale, FMK_TSO_FEBRUARY);
				case TSO_MARCH:
					return this->formatMessage(locale, FMK_TSO_MARCH);
				case TSO_APRIL:
					return this->formatMessage(locale, FMK_TSO_APRIL);
				case TSO_MAY:
					return this->formatMessage(locale, FMK_TSO_MAY);
				case TSO_JUNE:
					return this->formatMessage(locale, FMK_TSO_JUNE);
				case TSO_JULY:
					return this->formatMessage(locale, FMK_TSO_JULY);
				case TSO_AUGUST:
					return this->formatMessage(locale, FMK_TSO_AUGUST);
				case TSO_SEPTEMBER:
					return this->formatMessage(locale, FMK_TSO_SEPTEMBER);
				case TSO_OCTOBER:
					return this->formatMessage(locale, FMK_TSO_OCTOBER);
				case TSO_NOVEMBER:
					return this->formatMessage(locale, FMK_TSO_NOVEMBER);
				case TSO_DECEMBER:
					return this->formatMessage(locale, FMK_TSO_DECEMBER);
				case TSO_AM:
					return this->formatMessage(locale, FMK_TSO_AM);
				case TSO_PM:
					return this->formatMessage(locale, FMK_TSO_PM);
				default:
					throw error::ProgrammingError("Unrecognized time string option in "
							"FormatMessageCatalog::getTimeOptionString(): "
							+ util::StringUtils::toString(static_cast<int>(option)));
			}
		}

		String getTimeFormat(const locale::Locale& locale, LocalizedTimeFormat format) {
			switch(format) {
				case LTF_SHORT_DATE:
					return this->formatMessage(locale, FMK_FORMAT_SHORT_DATE);
				case LTF_MEDIUM_DATE:
					return this->formatMessage(locale, FMK_FORMAT_MEDIUM_DATE);
				case LTF_LONG_DATE:
					return this->formatMessage(locale, FMK_FORMAT_LONG_DATE);
				case LTF_FULL_DATE:
					return this->formatMessage(locale, FMK_FORMAT_FULL_DATE);
				case LTF_SHORT_TIME:
					return this->formatMessage(locale, FMK_FORMAT_SHORT_TIME);
				case LTF_MEDIUM_TIME:
					return this->formatMessage(locale, FMK_FORMAT_MEDIUM_TIME);
				case LTF_LONG_TIME:
					return this->formatMessage(locale, FMK_FORMAT_LONG_TIME);
				case LTF_FULL_TIME:
					return this->formatMessage(locale, FMK_FORMAT_FULL_TIME);
				case LTF_SHORT_DATETIME:
					return this->formatMessage(locale, FMK_FORMAT_SHORT_DATETIME);
				case LTF_MEDIUM_DATETIME:
					return this->formatMessage(locale, FMK_FORMAT_MEDIUM_DATETIME);
				case LTF_LONG_DATETIME:
					return this->formatMessage(locale, FMK_FORMAT_LONG_DATETIME);
				case LTF_FULL_DATETIME:
					return this->formatMessage(locale, FMK_FORMAT_FULL_DATETIME);
				default:
					throw error::ProgrammingError("Unrecognized time format in "
							"FormatMessageCatalog::getTimeFormat(): "
							+ util::StringUtils::toString(static_cast<int>(format)));
			}
		}

		template<CalendarPrecision Precision, CalendarModulus Modulus>
		String formatTime(const locale::Locale& locale,
				const String& format, const TimePoint<Precision, Modulus>& point) {
			FormatMessageCatalogTimeStringEmitter stringEmitter(*this, locale);
			TimeFormatter<
				CharT,
				TimeNumericRenditionT,
				TimeIntegerFormatterT,
				TimeFormatRenditionT
			> formatter(stringEmitter);
			return formatter.formatTime(format, point);
		}

		template<CalendarPrecision Precision, CalendarModulus Modulus>
		String formatTime(const locale::Locale& locale,
				LocalizedTimeFormat format, const TimePoint<Precision, Modulus>& point) {
			FormatMessageCatalogTimeStringEmitter stringEmitter(*this, locale);
			TimeFormatter<
				CharT,
				TimeNumericRenditionT,
				TimeIntegerFormatterT,
				TimeFormatRenditionT
			> formatter(stringEmitter);
			return formatter.formatTime(getTimeFormat(locale, format), point);
		}

		virtual std::string getSubjectComponentName() const {
			return "RedStrain/Calendar/Formats";
		}

	};

	typedef FormatMessageCatalog<text::Char16, platform::PropertyMutexLockingPolicy> FormatMessageCatalog16;

	REDSTRAIN_CALENDAR_API FormatMessageCatalog16& getDefaultFormatMessageCatalog16();

}}

#endif /* REDSTRAIN_MOD_CALENDAR_FORMATMESSAGECATALOG_HPP */
