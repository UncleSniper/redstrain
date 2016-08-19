#ifndef REDSTRAIN_MOD_UTIL_FIREEVENTAPPENDER_HPP
#define REDSTRAIN_MOD_UTIL_FIREEVENTAPPENDER_HPP

#include "Appender.hpp"

namespace redengine {
namespace util {

	template<typename ListenerT, typename EventT>
	class FireEventAppender : public Appender<ListenerT*> {

	  public:
		typedef ListenerT Listener;
		typedef EventT Event;
		typedef void (ListenerT::*Callback)(EventT&);

	  private:
		const Callback callback;
		EventT& event;

	  public:
		FireEventAppender(Callback callback, EventT& event) : callback(callback), event(event) {}

		FireEventAppender(const FireEventAppender& appender)
				: Appender<ListenerT*>(appender), callback(appender.callback), event(appender.event) {}

		inline Callback getCallback() const {
			return callback;
		}

		inline EventT& getEvent() {
			return event;
		}

		inline const EventT& getEvent() const {
			return event;
		}

		virtual void append(ListenerT *const& listener) {
			(listener->*callback)(event);
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_FIREEVENTAPPENDER_HPP */
