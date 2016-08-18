#ifndef REDSTRAIN_MOD_UTIL_LISTENERLIST_HPP
#define REDSTRAIN_MOD_UTIL_LISTENERLIST_HPP

#include <list>

#include "AbstractListeners.hpp"

namespace redengine {
namespace util {

	template<typename ListenerT>
	class ListenerList : public AbstractListeners<ListenerT, std::list<ListenerT*> > {

	  public:
		ListenerList() {}

		ListenerList(const ListenerList& listeners)
				: AbstractListeners<ListenerT, std::list<ListenerT*> >(listeners) {}

		virtual bool addListener(ListenerT& listener) {
			this->listeners.push_back(&listener);
			listener.ref();
			return true;
		}

		virtual bool removeListener(ListenerT& listener) {
			typename std::list<ListenerT*>::iterator begin(this->listeners.begin()), end(this->listeners.end());
			for(; begin != end; ++begin) {
				if(*begin == &listener) {
					this->listeners.erase(begin);
					listener.unref();
					return true;
				}
			}
			return false;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_LISTENERLIST_HPP */
