#ifndef REDSTRAIN_MOD_ERROR_STACKTRACESINKBASE_HPP
#define REDSTRAIN_MOD_ERROR_STACKTRACESINKBASE_HPP

#include <redstrain/redmond/unmangle/CPPSymbol.hpp>

#include "StackTraceSink.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StackTraceSinkBase : public virtual StackTraceSink {

	  protected:
		virtual void beginHeader() = 0;
		virtual void endHeader() = 0;
		virtual void beginFrame() = 0;
		virtual void writeSymbol(const redmond::unmangle::CPPSymbol&) = 0;
		virtual void beginFrameModule(util::MemorySize) = 0;
		virtual void endFrameModule() = 0;
		virtual void endFrame() = 0;

	  public:
		StackTraceSinkBase();
		StackTraceSinkBase(const StackTraceSinkBase&);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STACKTRACESINKBASE_HPP */
