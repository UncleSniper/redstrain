#ifndef REDSTRAIN_MOD_UTIL_MAXIMUMAPPENDER_HPP
#define REDSTRAIN_MOD_UTIL_MAXIMUMAPPENDER_HPP

#include "Appender.hpp"
#include "IntegerBounds.hpp"

namespace redengine {
namespace util {

	template<
		typename IntegerT,
		IntegerT Infinity = IntegerBounds<IntegerT>::MAX
	>
	class MaximumAppender : public Appender<IntegerT> {

	  private:
		IntegerT maximum;
		bool hasMaximum;

	  public:
		MaximumAppender() : maximum(static_cast<IntegerT>(0)), hasMaximum(false) {}

		MaximumAppender(const MaximumAppender& appender)
				: Appender<IntegerT>(appender), maximum(appender.maximum), hasMaximum(appender.hasMaximum) {}

		IntegerT getMaximum() const {
			return hasMaximum ? maximum : Infinity;
		}

		virtual void append(const IntegerT& current) {
			if(!hasMaximum || current > maximum) {
				maximum = current;
				hasMaximum = true;
			}
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_MAXIMUMAPPENDER_HPP */
