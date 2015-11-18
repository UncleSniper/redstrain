#ifndef REDSTRAIN_MOD_LOCALE_COREMESSAGECATALOG_HPP
#define REDSTRAIN_MOD_LOCALE_COREMESSAGECATALOG_HPP

#include <redstrain/platform/PropertyMutexLockingPolicy.hpp>
#include <redstrain/text/types.hpp>

#include "CoreMessageKey.hpp"
#include "FormattedMessageCatalog.hpp"

namespace redengine {
namespace locale {

	template<
		typename CharT,
		typename LockingPolicyT,
		typename NumericRenditionT
			= typename FormattedMessageCatalog<CharT, CoreMessageKey, LockingPolicyT>::Formatter::NumericRendition,
		typename IntegerFormatterT
			= typename FormattedMessageCatalog<CharT, CoreMessageKey, LockingPolicyT>::Formatter::IntegerFormatter,
		typename FloatFormatterT
			= typename FormattedMessageCatalog<CharT, CoreMessageKey, LockingPolicyT>::Formatter::FloatFormatter
	>
	class CoreMessageCatalog : public FormattedMessageCatalog<
		CharT,
		CoreMessageKey,
		LockingPolicyT,
		NumericRenditionT,
		IntegerFormatterT,
		FloatFormatterT
	> {

	  public:
		typedef std::basic_string<CharT> String;

	  public:
		CoreMessageCatalog() {}

		CoreMessageCatalog(typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: FormattedMessageCatalog<CharT, CoreMessageKey, LockingPolicyT, NumericRenditionT,
				IntegerFormatterT, FloatFormatterT>(lockInitializer) {}

		CoreMessageCatalog(const CoreMessageCatalog& catalog)
				: FormattedMessageCatalog<CharT, CoreMessageKey, LockingPolicyT, NumericRenditionT,
				IntegerFormatterT, FloatFormatterT>(catalog) {}

		CharT getDecimalPoint(const Locale& locale) {
			String str(this->formatMessage(locale, CMK_DECIMAL_POINT));
			return str.empty() ? NumericRenditionT::DECIMAL_POINT : str[0];
		}

		CharT getGroupSeparator(const Locale& locale) {
			String str(this->formatMessage(locale, CMK_GROUP_SEPARATOR));
			return str.empty() ? NumericRenditionT::GROUP_SEPARATOR : str[0];
		}

		String getFormattingOptionString(const Locale& locale, text::FormattingStringOption option) {
			switch(option) {
				case text::FSO_NAN:
					return this->formatMessage(locale, CMK_FLOAT_NAN);
				case text::FSO_POSITIVE_INFINITY:
					return this->formatMessage(locale, CMK_FLOAT_POSITIVE_INFINITY);
				case text::FSO_NEGATIVE_INFINITY:
					return this->formatMessage(locale, CMK_FLOAT_NEGATIVE_INFINITY);
				default:
					throw error::ProgrammingError("Unrecognized formatting string option in "
							"CoreMessageCatalog::emitString(): "
							+ util::StringUtils::toString(static_cast<int>(option)));
			}
		}

	};

	typedef CoreMessageCatalog<text::Char16, platform::PropertyMutexLockingPolicy> CoreMessageCatalog16;

	CoreMessageCatalog16& getDefaultCoreMessageCatalog16();

}}

#endif /* REDSTRAIN_MOD_LOCALE_COREMESSAGECATALOG_HPP */
