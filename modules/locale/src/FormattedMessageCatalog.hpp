#ifndef REDSTRAIN_MOD_LOCALE_FORMATTEDMESSAGECATALOG_HPP
#define REDSTRAIN_MOD_LOCALE_FORMATTEDMESSAGECATALOG_HPP

#include <redstrain/util/Unref.hpp>
#include <redstrain/text/Formatter.hpp>

#include "MessageCatalog.hpp"
#include "MessageFormattingError.hpp"

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

#define REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR \
	catch(const text::FormattingError& formattingError) { \
		util::Unref<text::FormattingError> fecopy(formattingError.cloneAs<text::FormattingError>()); \
		throw MessageFormattingError(*fecopy, locale, \
				static_cast<util::MemorySize>(key), this->getSubjectComponentName()); \
	}

		template<typename IteratorT>
		String formatMessageIter(const Locale& locale, KeyT key, const IteratorT& beginItems,
				util::MemorySize itemCount) {
			try {
				return formatter.template formatIter<IteratorT>(this->getMessage(locale, key), beginItems, itemCount);
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
		}

		String formatMessage(const Locale& locale, KeyT key, const std::list<const Item*>& items) {
			try {
				return formatter.format(this->getMessage(locale, key), items);
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
		}

		String formatMessage(const Locale& locale, KeyT key, const std::vector<const Item*>& items) {
			try {
				return formatter.format(this->getMessage(locale, key), items);
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
		}

		String formatMessage(const Locale& locale, KeyT key) {
			try {
				return formatter.format(this->getMessage(locale, key));
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0
		) {
			try {
				return formatter.format(
					this->getMessage(locale, key),
					item0
				);
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1
		) {
			try {
				return formatter.format(
					this->getMessage(locale, key),
					item0,
					item1
				);
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1,
			const Item& item2
		) {
			try {
				return formatter.format(
					this->getMessage(locale, key),
					item0,
					item1,
					item2
				);
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
		}

		String formatMessage(
			const Locale& locale,
			KeyT key,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3
		) {
			try {
				return formatter.format(
					this->getMessage(locale, key),
					item0,
					item1,
					item2,
					item3
				);
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
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
			try {
				return formatter.format(
					this->getMessage(locale, key),
					item0,
					item1,
					item2,
					item3,
					item4
				);
			}
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
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
			try {
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
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
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
			try {
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
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
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
			try {
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
			REDSTRAIN_FORMATTED_MESSAGE_CATALOG_HANDLE_FORMATTING_ERROR
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_FORMATTEDMESSAGECATALOG_HPP */
