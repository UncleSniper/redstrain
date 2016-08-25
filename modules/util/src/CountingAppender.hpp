#include "Appender.hpp"

namespace redengine {
namespace util {

	template<typename ElementT, typename CounterT>
	class CountingAppender : public Appender<ElementT> {

	  public:
		typedef CounterT Counter;

	  private:
		CounterT count;

	  public:
		CountingAppender(CounterT count = static_cast<CounterT>(0)) : count(count) {}
		CountingAppender(const CountingAppender& appender) : Appender<ElementT>(appender), count(appender.count) {}

		inline CounterT getCount() const {
			return count;
		}

		inline void setCount(CounterT count) {
			this->count = count;
		}

		virtual void append(const ElementT&) {
			++count;
		}

	};

}}
