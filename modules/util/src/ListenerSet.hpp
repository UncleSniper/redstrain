#ifndef REDSTRAIN_MOD_UTIL_LISTENERSET_HPP
#define REDSTRAIN_MOD_UTIL_LISTENERSET_HPP

#include <set>

#include "AbstractListeners.hpp"

namespace redengine {
namespace util {

	template<typename ListenerT>
	class ListenerSet : public AbstractListeners<ListenerT, std::set<ListenerT*> > {

	  public:
		ListenerSet() {}

		ListenerSet(const ListenerSet& listeners)
				: AbstractListeners<ListenerT, std::set<ListenerT*> >(listeners) {}

		virtual bool addListener(ListenerT& listener) {
			if(!this->listeners.insert(&listener).second)
				return false;
			listener.ref();
			return true;
		}

		virtual bool removeListener(ListenerT& listener) {
			if(!this->listeners.erase(&listener))
				return false;
			listener.unref();
			return true;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_LISTENERSET_HPP */
