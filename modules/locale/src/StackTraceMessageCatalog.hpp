#ifndef REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGECATALOG_HPP
#define REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGECATALOG_HPP

#include <redstrain/platform/PropertyMutexLockingPolicy.hpp>
#include <redstrain/text/types.hpp>

#include "StackTraceMessageSource.hpp"
#include "FormattedMessageCatalog.hpp"

namespace redengine {
namespace locale {

	template<
		typename CharT,
		typename LockingPolicyT,
		typename NumericRenditionT
			= typename FormattedMessageCatalog<
				CharT,
				StackTraceMessageKey,
				LockingPolicyT
			>::Formatter::NumericRendition,
		typename IntegerFormatterT
			= typename FormattedMessageCatalog<
				CharT,
				StackTraceMessageKey,
				LockingPolicyT
			>::Formatter::IntegerFormatter,
		typename FloatFormatterT
			= typename FormattedMessageCatalog<
				CharT,
				StackTraceMessageKey,
				LockingPolicyT
			>::Formatter::FloatFormatter
	>
	class StackTraceMessageCatalog : public FormattedMessageCatalog<
		CharT,
		StackTraceMessageKey,
		LockingPolicyT,
		NumericRenditionT,
		IntegerFormatterT,
		FloatFormatterT
	>, public StackTraceMessageSource<CharT> {

	  public:
		typedef std::basic_string<CharT> String;

	  public:
		StackTraceMessageCatalog() {}

		StackTraceMessageCatalog(typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: FormattedMessageCatalog<CharT, StackTraceMessageKey, LockingPolicyT, NumericRenditionT,
				IntegerFormatterT, FloatFormatterT>(lockInitializer) {}

		StackTraceMessageCatalog(const StackTraceMessageCatalog& catalog)
				: FormattedMessageCatalog<CharT, StackTraceMessageKey, LockingPolicyT, NumericRenditionT,
				IntegerFormatterT, FloatFormatterT>(catalog), StackTraceMessageSource<CharT>(catalog) {}

		virtual String localizeMessage(const Locale& locale, StackTraceMessageKey key) {
			return this->formatMessage(locale, key);
		}

	};

	typedef StackTraceMessageCatalog<text::Char16, platform::PropertyMutexLockingPolicy> StackTraceMessageCatalog16;

	REDSTRAIN_LOCALE_API StackTraceMessageCatalog16& getDefaultStackTraceMessageCatalog16();

}}

#endif /* REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGECATALOG_HPP */
