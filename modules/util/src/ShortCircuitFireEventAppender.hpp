#ifndef REDSTRAIN_MOD_UTIL_SHORTCIRCUITFIREEVENTAPPENDER_HPP
#define REDSTRAIN_MOD_UTIL_SHORTCIRCUITFIREEVENTAPPENDER_HPP

#include "Appender.hpp"

namespace redengine {
namespace util {

	template<typename ListenerT, typename EventT, typename ReturnT = bool>
	class ShortCircuitFireEventAppender : public Appender<ListenerT*> {

	  public:
		typedef ListenerT Listener;
		typedef EventT Event;
		typedef ReturnT (ListenerT::*Callback)(EventT&);

	  private:
		const Callback callback;
		EventT& event;
		const bool stopSignal, resultIfStopped;
		bool currentResult;

	  public:
		ShortCircuitFireEventAppender(Callback callback, EventT& event, bool stopSignal, bool resultIfStopped)
				: callback(callback), event(event), stopSignal(stopSignal), resultIfStopped(resultIfStopped),
				currentResult(!resultIfStopped) {}

		ShortCircuitFireEventAppender(const ShortCircuitFireEventAppender& appender)
				: Appender<ListenerT*>(appender), callback(appender.callback), event(appender.event),
				stopSignal(appender.stopSignal), resultIfStopped(appender.resultIfStopped),
				currentResult(appender.currentResult) {}

		inline Callback getCallback() const {
			return callback;
		}

		inline EventT& getEvent() {
			return event;
		}

		inline const EventT& getEvent() const {
			return event;
		}

		inline bool getStopSignal() const {
			return stopSignal;
		}

		inline bool getResultIfStopped() const {
			return resultIfStopped;
		}

		inline bool getResult() const {
			return currentResult;
		}

		virtual void append(ListenerT *const& listener) {
			if(currentResult == resultIfStopped)
				return;
			// Casting to bool would probably be the right thing to do...
			// We'll do it this way, just to be perfectly safe in that
			// 'true' will most certainly equate one (seeing as some
			// operator bool() might return something that is numerically
			// neither zero nor one). The one thing to NOT do is use
			// operator! to boolize the result; who knows what type that
			// might return...
			bool result = (listener->*callback)(event) ? true : false;
			if(result != !stopSignal)
				currentResult = resultIfStopped;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_SHORTCIRCUITFIREEVENTAPPENDER_HPP */
