#ifndef REDSTRAIN_MOD_UTIL_LISTENERS_HPP
#define REDSTRAIN_MOD_UTIL_LISTENERS_HPP

#include "Appender.hpp"
#include "types.hpp"

namespace redengine {
namespace util {

	template<typename ListenerT>
	class Listeners {

	  public:
		typedef ListenerT Listener;

	  public:
		Listeners() {}
		Listeners(const Listeners&) {}
		virtual ~Listeners() {}

		virtual void getListeners(Appender<ListenerT*>&) const = 0;
		virtual bool addListener(ListenerT&) = 0;
		virtual bool removeListener(ListenerT&) = 0;
		virtual void clearListeners() = 0;
		virtual MemorySize getListenerCount() const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_LISTENERS_HPP */
