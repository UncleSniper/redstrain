#ifndef REDSTRAIN_MOD_LOCALE_MESSAGECATALOG_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGECATALOG_HPP

#include "MessageCache.hpp"

namespace redengine {
namespace locale {

	class Locale;

	template<typename CharT, typename KeyT>
	class MessageCatalog {

	  public:
		typedef CharT Char;
		typedef KeyT Key;

	  private:
		const Locale* lastLocale;
		MessageCache<CharT>* cache;

	  public:
		MessageCatalog() : lastLocale(NULL), cache(NULL) {}
		MessageCatalog(const MessageCatalog&) : lastLocale(NULL), cache(NULL) {}
		virtual ~MessageCatalog() {}

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGECATALOG_HPP */
