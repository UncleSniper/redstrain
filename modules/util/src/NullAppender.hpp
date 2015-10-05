#ifndef REDSTRAIN_MOD_UTIL_NULLAPPENDER_HPP
#define REDSTRAIN_MOD_UTIL_NULLAPPENDER_HPP

#include "Appender.hpp"

namespace redengine {
namespace util {

	template<typename ElementT>
	class NullAppender : public Appender<ElementT> {

	  public:
		NullAppender() {}
		NullAppender(const NullAppender& appender) : Appender<ElementT>(appender) {}

		virtual void append(const ElementT&) {}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_NULLAPPENDER_HPP */
