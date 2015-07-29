#ifndef REDSTRAIN_MOD_UTIL_MINIMUMAPPENDER_HPP
#define REDSTRAIN_MOD_UTIL_MINIMUMAPPENDER_HPP

#include "Appender.hpp"
#include "IntegerBounds.hpp"

namespace redengine {
namespace util {

	template<
		typename IntegerT,
		IntegerT NegativeInfinity = IntegerBounds<IntegerT>::MIN
	>
	class MinimumAppender : public Appender<IntegerT> {

	  private:
		IntegerT minimum;
		bool hasMinimum;

	  public:
		MinimumAppender() : minimum(static_cast<IntegerT>(0)), hasMinimum(false) {}

		MinimumAppender(const MinimumAppender& appender)
				: Appender<IntegerT>(appender), minimum(appender.minimum), hasMinimum(appender.hasMinimum) {}

		IntegerT getMinimum() const {
			return hasMinimum ? minimum : NegativeInfinity;
		}

		virtual void append(const IntegerT& current) {
			if(!hasMinimum || current < minimum) {
				minimum = current;
				hasMinimum = true;
			}
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_MINIMUMAPPENDER_HPP */
