#ifndef REDSTRAIN_MOD_TEXT_STACKTRACESINKBASE16_HPP
#define REDSTRAIN_MOD_TEXT_STACKTRACESINKBASE16_HPP

#include <redstrain/error/StackTraceSinkBase.hpp>
#include <redstrain/error/HighlightingStackTraceSink.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API StackTraceSinkBase16
			: public virtual error::StackTraceSinkBase, public virtual error::HighlightingStackTraceSink {

	  protected:
		static const String16 msg_noStackTraceAvailable;
		static const String16 msg_stackTraceHeader;
		static const String16 msg_noFramesOnStack;
		static const String16 msg_frameNotReconstructible;
		static const String16 msg_beforeModuleName;
		static const String16 msg_afterModuleName;
		static const String16 msg_unresolvedSymbol;
		static const String16 msg_startBottomOfUnwinding;
		static const String16 msg_endBottomOfUnwinding;

	  protected:
		virtual void writeHeader(const String16&) = 0;
		virtual void writeFrame(const String16&) = 0;
		virtual void writeFrameModule(const String16&) = 0;
		virtual void writeNoStackTraceAvailable();
		virtual void writeStackTraceHeader();
		virtual void writeNoFramesOnStack();
		virtual void writeFrameNotReconstructible();
		virtual void writeBeforeModuleName();
		virtual void writeAfterModuleName();
		virtual void writeUnresolvedSymbol();
		virtual void writeBottomOfUnwinding(util::MemorySize);

	  public:
		StackTraceSinkBase16();
		StackTraceSinkBase16(const StackTraceSinkBase16&);

		virtual void noStackTraceAvailable();
		virtual void beginStackTrace(util::MemorySize);
		virtual void putStackFrame(void*, util::MemorySize, util::MemorySize);
		virtual void endStackTrace(util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_STACKTRACESINKBASE16_HPP */
