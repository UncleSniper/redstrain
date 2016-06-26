#ifndef REDSTRAIN_MOD_ERROR_STACKTRACESINK_HPP
#define REDSTRAIN_MOD_ERROR_STACKTRACESINK_HPP

#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StackTraceSink {

	  public:
		StackTraceSink();
		StackTraceSink(const StackTraceSink&);
		virtual ~StackTraceSink();

		virtual void noStackTraceAvailable() = 0;
		virtual void beginStackTrace(util::MemorySize) = 0;
		virtual void putStackFrame(void*, util::MemorySize, util::MemorySize) = 0;
		virtual void endStackTrace(util::MemorySize) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STACKTRACESINK_HPP */
