#ifndef REDSTRAIN_MOD_ERROR_STACKTRACESINKBASE8_HPP
#define REDSTRAIN_MOD_ERROR_STACKTRACESINKBASE8_HPP

#include <string>

#include "StackTraceSinkBase.hpp"
#include "HighlightingStackTraceSink.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StackTraceSinkBase8
			: public virtual StackTraceSinkBase, public HighlightingStackTraceSink {

	  protected:
		virtual void writeHeader(const std::string&) = 0;
		virtual void writeFrame(const std::string&) = 0;
		virtual void writeFrameModule(const std::string&) = 0;

	  public:
		StackTraceSinkBase8();
		StackTraceSinkBase8(const StackTraceSinkBase8&);

		virtual void noStackTraceAvailable();
		virtual void beginStackTrace(util::MemorySize);
		virtual void putStackFrame(void*, util::MemorySize, util::MemorySize);
		virtual void endStackTrace(util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STACKTRACESINKBASE8_HPP */
