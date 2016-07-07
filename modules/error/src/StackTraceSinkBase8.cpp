#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/redmond/unmangle/CPPSymbol.hpp>
#include <redstrain/redmond/unmangle.hpp>

#include "StackTrace.hpp"
#include "StackTraceSinkBase8.hpp"

using std::string;
using redengine::util::Delete;
using redengine::util::MemorySize;
using redengine::util::StringUtils;
using redengine::redmond::unmangle::CPPSymbol;
using redengine::redmond::unmangleCPPSymbol_native;

namespace redengine {
namespace error {

	StackTraceSinkBase8::StackTraceSinkBase8() {}

	StackTraceSinkBase8::StackTraceSinkBase8(const StackTraceSinkBase8& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink) {}

	void StackTraceSinkBase8::noStackTraceAvailable() {
		beginHeader();
		highlightNoTraceAvailable();
		writeHeader("No stack trace available.");
		endHighlighting();
		endHeader();
	}

	void StackTraceSinkBase8::beginStackTrace(MemorySize frameCount) {
		beginHeader();
		writeHeader("Stack trace (innermost call first):");
		endHeader();
		if(!frameCount) {
			beginFrame();
			highlightNoFramesOnStack();
			writeFrame("(no frames on stack)");
			endHighlighting();
			endFrame();
		}
	}

	void StackTraceSinkBase8::putStackFrame(void* returnAddress, MemorySize, MemorySize) {
		if(!returnAddress) {
			beginFrame();
			highlightFrameNotReconstructible();
			writeFrame("<frame not reconstructible>");
			endHighlighting();
			endFrame();
			return;
		}
		beginFrame();
		string rastr(StackTrace::formatMemoryAddress(returnAddress));
		highlightMemoryAddress();
		writeFrame(rastr);
		endHighlighting();
		writeFrame(" ");
		void *moduleBase, *symbolAddress;
		string moduleName, symbolName;
		if(StackTrace::getSymbolInfoForReturnAddress(returnAddress,
				moduleName, moduleBase, symbolName, symbolAddress)) {
			Delete<CPPSymbol> umsymbol(unmangleCPPSymbol_native(symbolName));
			if(*umsymbol)
				writeSymbol(**umsymbol);
			else
				writeFrame(symbolName);
			writeFrame(" [");
			highlightMemoryAddress();
			writeFrame(StackTrace::formatMemoryAddress(symbolAddress));
			endHighlighting();
			writeFrame(" + ");
			highlightMemoryAddress();
			writeFrame(StackTrace::formatMemoryAddress(StackTrace::addressDifference(returnAddress, symbolAddress)));
			endHighlighting();
			writeFrame("]");
			if(!moduleName.empty()) {
				beginFrameModule(static_cast<MemorySize>(rastr.length()) + static_cast<MemorySize>(1u));
				writeFrameModule("within ");
				highlightModuleName();
				writeFrameModule(moduleName);
				endHighlighting();
				writeFrameModule(" [");
				highlightMemoryAddress();
				writeFrameModule(StackTrace::formatMemoryAddress(moduleBase));
				endHighlighting();
				writeFrameModule(" + ");
				highlightMemoryAddress();
				writeFrameModule(StackTrace::formatMemoryAddress(StackTrace::addressDifference(returnAddress,
						moduleBase)));
				endHighlighting();
				writeFrameModule("]");
				endFrameModule();
			}
		}
		else {
			highlightUnresolvedSymbol();
			writeFrame("<unresolved symbol>");
			endHighlighting();
		}
		endFrame();
	}

	void StackTraceSinkBase8::endStackTrace(MemorySize frameCount) {
		beginFrame();
		highlightBottomOfUnwinding();
		writeFrame("(Bottom of unwinding reached after " + StringUtils::toString(frameCount) + " frames.)");
		endHighlighting();
		endFrame();
	}

}}
