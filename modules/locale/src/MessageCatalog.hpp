#ifndef REDSTRAIN_MOD_LOCALE_MESSAGECATALOG_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGECATALOG_HPP

#include <list>
#include <redstrain/platform/PolicyLocker.hpp>

#include "MessageLoader.hpp"
#include "NoMessagesForRequestedLocaleError.hpp"

namespace redengine {
namespace locale {

	class Locale;

	template<typename CharT, typename KeyT, typename LockingPolicyT>
	class MessageCatalog : protected LockingPolicyT {

	  private:
		typedef std::list<MessageLoader<CharT>*> MessageLoaders;

	  public:
		typedef CharT Char;
		typedef KeyT Key;
		typedef LockingPolicyT LockingPolicy;
		typedef std::basic_string<CharT> String;
		typedef typename MessageLoaders::const_iterator MessageLoaderIterator;

	  protected:
		typedef platform::PolicyLocker<LockingPolicyT> CatalogLocker;

	  private:
		const Locale* lastLocale;
		MessageCache<CharT>* cache;
		MessageLoaders loaders;

	  private:
		MessageCache<CharT>* loadMessageCache(const Locale& locale) const {
			MessageLoaderIterator begin(loaders.begin()), end(loaders.end());
			for(; begin != end; ++begin) {
				MessageCache<CharT>* mc = (*begin)->loadMessages(locale);
				if(mc)
					return mc;
			}
			return NULL;
		}

	  protected:
		virtual void localeSwitched() {}

	  public:
		MessageCatalog() : lastLocale(NULL), cache(NULL) {}

		MessageCatalog(typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: LockingPolicyT(lockInitializer), lastLocale(NULL), cache(NULL) {}

		MessageCatalog(const MessageCatalog& catalog)
				: LockingPolicyT(catalog), lastLocale(NULL), cache(NULL), loaders(catalog.loaders) {
			MessageLoaderIterator mlbegin(loaders.begin()), mlend(loaders.end());
			for(; mlbegin != mlend; ++mlbegin)
				(*mlbegin)->ref();
		}

		virtual ~MessageCatalog() {
			MessageLoaderIterator mlbegin(loaders.begin()), mlend(loaders.end());
			for(; mlbegin != mlend; ++mlbegin)
				(*mlbegin)->unref();
		}

		void addMessageLoader(MessageLoader<CharT>& loader) {
			loaders.push_back(&loader);
			loader.ref();
		}

		bool removeMessageLoader(const MessageLoader<CharT>& loader) {
			typename std::list<MessageLoader<CharT>*>::iterator begin(loaders.begin()), end(loaders.end());
			for(; begin != end; ++begin) {
				if(*begin == &loader) {
					(*begin)->unref();
					loaders.erase(begin);
					return true;
				}
			}
			return false;
		}

		void clearMessageLoaders() {
			MessageLoaderIterator begin(loaders.begin()), end(loaders.end());
			for(; begin != end; ++begin)
				(*begin)->unref();
			loaders.clear();
		}

		void getMessageLoaders(MessageLoaderIterator& begin, MessageLoaderIterator& end) const {
			begin = loaders.begin();
			end = loaders.end();
		}

		inline const Locale* getCurrentLocale() const {
			return lastLocale;
		}

		void purgeCache() {
			CatalogLocker lock(this);
			bool hadLocale = !!lastLocale;
			lastLocale = NULL;
			if(cache) {
				delete cache;
				cache = NULL;
			}
			if(hadLocale)
				localeSwitched();
			lock.release();
		}

		String getMessage(const Locale& locale, KeyT key) {
			CatalogLocker lock(this);
			if(&locale != lastLocale) {
				lastLocale = NULL;
				if(cache) {
					delete cache;
					cache = NULL;
				}
				cache = loadMessageCache(locale);
				if(cache)
					lastLocale = &locale;
				else {
					lock.release();
					throw NoMessagesForRequestedLocaleError(locale);
				}
				localeSwitched();
			}
			String msg(cache->getMessage(static_cast<size_t>(key)).getValue());
			lock.release();
			return msg;
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGECATALOG_HPP */
