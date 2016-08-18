#ifndef REDSTRAIN_MOD_UTIL_ABSTRACTLISTENERS_HPP
#define REDSTRAIN_MOD_UTIL_ABSTRACTLISTENERS_HPP

#include "Listeners.hpp"

namespace redengine {
namespace util {

	template<typename ListenerT, typename CollectionT>
	class AbstractListeners : public Listeners<ListenerT> {

	  protected:
		typedef CollectionT Collection;
		typedef typename CollectionT::const_iterator Iterator;

	  private:
		struct UnrefCollection {

			const CollectionT& collection;

			UnrefCollection(const CollectionT& collection) : collection(collection) {}

			~UnrefCollection() {
				Iterator begin(collection.begin()), end(collection.end());
				for(; begin != end; ++begin)
					(*begin)->unref();
			}

		};

	  protected:
		CollectionT listeners;

	  public:
		AbstractListeners() {}

		AbstractListeners(const AbstractListeners& listeners)
				: Listeners<ListenerT>(listeners), listeners(listeners.listeners) {
			Iterator begin(this->listeners.begin()), end(this->listeners.end());
			for(; begin != end; ++begin)
				(*begin)->ref();
		}

		virtual ~AbstractListeners() {
			Iterator begin(listeners.begin()), end(listeners.end());
			for(; begin != end; ++begin)
				(*begin)->unref();
		}

		virtual void getListeners(Appender<ListenerT*>& sink) const {
			CollectionT cache(listeners);
			Iterator begin(cache.begin()), end(cache.end());
			for(; begin != end; ++begin)
				(*begin)->ref();
			UnrefCollection unref(cache);
			for(begin = cache.begin(); begin != end; ++begin)
				sink.append(*begin);
			sink.doneAppending();
		}

		virtual void clearListeners() {
			Iterator begin(listeners.begin()), end(listeners.end());
			for(; begin != end; ++begin)
				(*begin)->unref();
			listeners.clear();
		}

		virtual MemorySize getListenerCount() const {
			return static_cast<MemorySize>(listeners.size());
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_ABSTRACTLISTENERS_HPP */
