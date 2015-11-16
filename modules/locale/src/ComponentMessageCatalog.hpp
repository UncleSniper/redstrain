#ifndef REDSTRAIN_MOD_LOCALE_COMPONENTMESSAGECATALOG_HPP
#define REDSTRAIN_MOD_LOCALE_COMPONENTMESSAGECATALOG_HPP

#include "CoreMessageCatalog.hpp"

namespace redengine {
namespace locale {

	template<
		typename CharT,
		typename KeyT,
		typename LockingPolicyT,
		typename CoreMessageCatalogLockingPolicyT = typename CoreMessageCatalog16::LockingPolicy,
		typename NumericRenditionT
			= typename FormattedMessageCatalog<CharT, KeyT, LockingPolicyT>::Formatter::NumericRendition,
		typename IntegerFormatterT
			= typename FormattedMessageCatalog<CharT, KeyT, LockingPolicyT>::Formatter::IntegerFormatter,
		typename FloatFormatterT
			= typename FormattedMessageCatalog<CharT, KeyT, LockingPolicyT>::Formatter::FloatFormatter,
		typename FormatRenditionT
			= typename FormattedMessageCatalog<CharT, KeyT, LockingPolicyT>::Formatter::FormatRendition,
		typename CoreMessageCatalogNumericRenditionT
			= typename FormattedMessageCatalog<
				CharT,
				CoreMessageKey,
				CoreMessageCatalogLockingPolicyT
			>::Formatter::NumericRendition,
		typename CoreMessageCatalogIntegerFormatterT
			= typename FormattedMessageCatalog<
				CharT,
				CoreMessageKey,
				CoreMessageCatalogLockingPolicyT
			>::Formatter::IntegerFormatter,
		typename CoreMessageCatalogFloatFormatterT
			= typename FormattedMessageCatalog<
				CharT,
				CoreMessageKey,
				CoreMessageCatalogLockingPolicyT
			>::Formatter::FloatFormatter
	>
	class ComponentMessageCatalog : public FormattedMessageCatalog<
		CharT,
		KeyT,
		LockingPolicyT,
		NumericRenditionT,
		IntegerFormatterT,
		FloatFormatterT,
		FormatRenditionT
	> {

	  public:
		typedef std::basic_string<CharT> String;
		typedef CoreMessageCatalog<
			CharT,
			CoreMessageCatalogLockingPolicyT,
			CoreMessageCatalogNumericRenditionT,
			CoreMessageCatalogIntegerFormatterT,
			CoreMessageCatalogFloatFormatterT
		> OptionMessageCatalog;
		typedef typename FormattedMessageCatalog<
			CharT,
			KeyT,
			LockingPolicyT,
			NumericRenditionT,
			IntegerFormatterT,
			FloatFormatterT,
			FormatRenditionT
		>::Formatter Formatter;

	  public:
		class CoreMessageCatalogFormattingOptionStringEmitter : public text::FormattingOptionStringEmitter<CharT> {

		  private:
			OptionMessageCatalog& optionCatalog;
			const Locale* currentLocale;

		  public:
			CoreMessageCatalogFormattingOptionStringEmitter(OptionMessageCatalog& optionCatalog)
					: optionCatalog(optionCatalog), currentLocale(NULL) {}

			CoreMessageCatalogFormattingOptionStringEmitter(const CoreMessageCatalogFormattingOptionStringEmitter&
					stringEmitter) : text::FormattingOptionStringEmitter<CharT>(stringEmitter),
					optionCatalog(stringEmitter.optionCatalog), currentLocale(stringEmitter.currentLocale) {}

			inline OptionMessageCatalog& getOptionMessageCatalog() {
				return optionCatalog;
			}

			inline const OptionMessageCatalog& getOptionMessageCatalog() const {
				return optionCatalog;
			}

			inline const Locale* getCurrentLocale() const {
				return currentLocale;
			}

			void setCurrentLocale(const Locale* locale) {
				currentLocale = locale ? &locale->intern() : NULL;
			}

			virtual String emitString(text::FormattingStringOption option) {
				if(currentLocale)
					return optionCatalog.getFormattingOptionString(*currentLocale, option);
				else
					return text::DefaultFormattingOptionStringEmitter<CharT>::instance.emitString(option);
			}

		};

	  private:
		CoreMessageCatalogFormattingOptionStringEmitter stringEmitter;

	  protected:
		virtual void localeSwitched() {
			const Locale* locale = this->getCurrentLocale();
			stringEmitter.setCurrentLocale(locale);
			if(locale) {
				// This might seem like a no-op, but we really should canonicalize
				// the Locale instance to what the setCurrentLocale() above made
				// it into, so the CoreMessageCatalog will always get the same
				// instance from us.
				locale = stringEmitter.getCurrentLocale();
				OptionMessageCatalog& optionCatalog = stringEmitter.getOptionMessageCatalog();
				typename Formatter::Options& options = this->getFormatter().getDefaultFormattingOptions();
				options.decimalPoint = optionCatalog.getDecimalPoint(*locale);
				options.groupSeparator = optionCatalog.getGroupSeparator(*locale);
			}
		}

	  public:
		ComponentMessageCatalog(OptionMessageCatalog& optionCatalog)
				: FormattedMessageCatalog<CharT, KeyT, LockingPolicyT, NumericRenditionT,
						IntegerFormatterT, FloatFormatterT, FormatRenditionT>(stringEmitter),
				stringEmitter(optionCatalog) {}

		ComponentMessageCatalog(OptionMessageCatalog& optionCatalog,
				typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: FormattedMessageCatalog<CharT, KeyT, LockingPolicyT, NumericRenditionT,
						IntegerFormatterT, FloatFormatterT, FormatRenditionT>(stringEmitter, lockInitializer),
				stringEmitter(optionCatalog) {}

		ComponentMessageCatalog(const ComponentMessageCatalog& catalog)
				: FormattedMessageCatalog<CharT, KeyT, LockingPolicyT, NumericRenditionT,
						IntegerFormatterT, FloatFormatterT, FormatRenditionT>(catalog),
				stringEmitter(catalog.stringEmitter) {}

		virtual ~ComponentMessageCatalog() {}

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_COMPONENTMESSAGECATALOG_HPP */
