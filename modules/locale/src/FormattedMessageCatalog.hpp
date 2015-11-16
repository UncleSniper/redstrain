#ifndef REDSTRAIN_MOD_LOCALE_FORMATTEDMESSAGECATALOG_HPP
#define REDSTRAIN_MOD_LOCALE_FORMATTEDMESSAGECATALOG_HPP

#include <redstrain/text/Formatter.hpp>

#include "MessageCatalog.hpp"

namespace redengine {
namespace locale {

	template<
		typename CharT,
		typename KeyT,
		typename LockingPolicyT,
		typename NumericRenditionT = typename text::Formatter<CharT>::NumericRendition,
		typename IntegerFormatterT = typename text::Formatter<CharT>::IntegerFormatter,
		typename FloatFormatterT = typename text::Formatter<CharT>::FloatFormatter,
		typename FormatRenditionT = typename text::Formatter<CharT>::FormatRendition
	>
	class FormattedMessageCatalog : public MessageCatalog<CharT, KeyT, LockingPolicyT> {

	  public:
		typedef text::Formatter<
			CharT,
			NumericRenditionT,
			IntegerFormatterT,
			FloatFormatterT,
			FormatRenditionT
		> Formatter;
		typedef std::basic_string<CharT> String;
		typedef typename Formatter::Item Item;

	  private:
		Formatter formatter;

	  public:
		FormattedMessageCatalog() {}

		FormattedMessageCatalog(text::FormattingOptionStringEmitter<CharT>& stringEmitter)
				: formatter(stringEmitter) {}

		FormattedMessageCatalog(typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: MessageCatalog<CharT, KeyT, LockingPolicyT>(lockInitializer) {}

		FormattedMessageCatalog(text::FormattingOptionStringEmitter<CharT>& stringEmitter,
				typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: MessageCatalog<CharT, KeyT, LockingPolicyT>(lockInitializer), formatter(stringEmitter) {}

		FormattedMessageCatalog(const FormattedMessageCatalog& catalog)
				: MessageCatalog<CharT, KeyT, LockingPolicyT>(catalog), formatter(catalog.formatter) {}

		virtual ~FormattedMessageCatalog() {}

		inline Formatter& getFormatter() {
			return formatter;
		}

		inline const Formatter& getFormatter() const {
			return formatter;
		}

		template<typename IteratorT>
		String formatMessageIter(const Locale& locale, KeyT key, const IteratorT& beginItems, size_t itemCount) {
			return formatter.template formatIter<IteratorT>(this->getMessage(locale, key), beginItems, itemCount);
		}

		String formatMessage(const Locale& locale, KeyT key, const std::list<const Item*>& items) {
			return formatter.format(this->getMessage(locale, key), items);
		}

		String formatMessage(const Locale& locale, KeyT key, const std::vector<const Item*>& items) {
			return formatter.format(this->getMessage(locale, key), items);
		}

		String formatMessage(const Locale& locale, KeyT key) {
			return formatter.format(this->getMessage(locale, key));
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0
		) {
			return formatter.format(
				this->getMessage(locale, key),
				item0
			);
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1
		) {
			return formatter.format(
				this->getMessage(locale, key),
				item0,
				item1
			);
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1,
			const Item& item2
		) {
			return formatter.format(
				this->getMessage(locale, key),
				item0,
				item1,
				item2
			);
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3
		) {
			return formatter.format(
				this->getMessage(locale, key),
				item0,
				item1,
				item2,
				item3
			);
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3,
			const Item& item4
		) {
			return formatter.format(
				this->getMessage(locale, key),
				item0,
				item1,
				item2,
				item3,
				item4
			);
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3,
			const Item& item4,
			const Item& item5
		) {
			return formatter.format(
				this->getMessage(locale, key),
				item0,
				item1,
				item2,
				item3,
				item4,
				item5
			);
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3,
			const Item& item4,
			const Item& item5,
			const Item& item6
		) {
			return formatter.format(
				this->getMessage(locale, key),
				item0,
				item1,
				item2,
				item3,
				item4,
				item5,
				item6
			);
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3,
			const Item& item4,
			const Item& item5,
			const Item& item6,
			const Item& item7
		) {
			return formatter.format(
				this->getMessage(locale, key),
				item0,
				item1,
				item2,
				item3,
				item4,
				item5,
				item6,
				item7
			);
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_FORMATTEDMESSAGECATALOG_HPP */
