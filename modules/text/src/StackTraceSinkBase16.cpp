#include <redstrain/util/Delete.hpp>
#include <redstrain/error/StackTrace.hpp>
#include <redstrain/redmond/unmangle/CPPSymbol.hpp>
#include <redstrain/redmond/unmangle.hpp>

#include "Transcode.hpp"
#include "StackTraceSinkBase16.hpp"
#include "DefaultIntegerFormatter.hpp"
#include "DefaultFormattingRendition.hpp"
#include "DefaultFormattingOptionStringEmitter.hpp"

using std::string;
using redengine::util::Delete;
using redengine::util::MemorySize;
using redengine::error::StackTrace;
using redengine::redmond::unmangle::CPPSymbol;
using redengine::redmond::unmangleCPPSymbol_native;

namespace redengine {
namespace text {

	const String16 StackTraceSinkBase16::msg_noStackTraceAvailable(
			Transcode::utf8ToBMP("No stack trace available."));
	const String16 StackTraceSinkBase16::msg_stackTraceHeader(
			Transcode::utf8ToBMP("Stack trace (innermost call first):"));
	const String16 StackTraceSinkBase16::msg_noFramesOnStack(Transcode::utf8ToBMP("(no frames on stack)"));
	const String16 StackTraceSinkBase16::msg_frameNotReconstructible(
			Transcode::utf8ToBMP("<frame not reconstructible>"));
	const String16 StackTraceSinkBase16::msg_beforeModuleName(Transcode::utf8ToBMP("within "));
	const String16 StackTraceSinkBase16::msg_afterModuleName;
	const String16 StackTraceSinkBase16::msg_unresolvedSymbol(Transcode::utf8ToBMP("<unresolved symbol>"));
	const String16 StackTraceSinkBase16::msg_startBottomOfUnwinding(
			Transcode::utf8ToBMP("(Bottom of unwinding reached after "));
	const String16 StackTraceSinkBase16::msg_endBottomOfUnwinding(Transcode::utf8ToBMP(" frames.)"));

	StackTraceSinkBase16::StackTraceSinkBase16() {}

	StackTraceSinkBase16::StackTraceSinkBase16(const StackTraceSinkBase16& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink) {}

	void StackTraceSinkBase16::noStackTraceAvailable() {
		beginHeader();
		highlightNoTraceAvailable();
		writeNoStackTraceAvailable();
		endHighlighting();
		endHeader();
	}

	void StackTraceSinkBase16::writeNoStackTraceAvailable() {
		writeHeader(StackTraceSinkBase16::msg_noStackTraceAvailable);
	}

	void StackTraceSinkBase16::beginStackTrace(MemorySize frameCount) {
		beginHeader();
		writeStackTraceHeader();
		endHeader();
		if(!frameCount) {
			beginFrame();
			highlightNoFramesOnStack();
			writeNoFramesOnStack();
			endHighlighting();
			endFrame();
		}
	}

	void StackTraceSinkBase16::writeStackTraceHeader() {
		writeHeader(StackTraceSinkBase16::msg_stackTraceHeader);
	}

	void StackTraceSinkBase16::writeNoFramesOnStack() {
		writeFrame(StackTraceSinkBase16::msg_noFramesOnStack);
	}

	static const String16 str_blank(Transcode::utf8ToBMP(" "));
	static const String16 str_blankLeftSquare(Transcode::utf8ToBMP(" ["));
	static const String16 str_blankPlusBlank(Transcode::utf8ToBMP(" + "));
	static const String16 str_rightSquare(Transcode::utf8ToBMP("]"));

	void StackTraceSinkBase16::putStackFrame(void* returnAddress, MemorySize, MemorySize) {
		if(!returnAddress) {
			beginFrame();
			highlightFrameNotReconstructible();
			writeFrameNotReconstructible();
			endHighlighting();
			endFrame();
			return;
		}
		beginFrame();
		string rastr(StackTrace::formatMemoryAddress(returnAddress));
		highlightMemoryAddress();
		writeFrame(Transcode::utf8ToBMP(rastr));
		endHighlighting();
		writeFrame(str_blank);
		void *moduleBase, *symbolAddress;
		string moduleName, symbolName;
		if(StackTrace::getSymbolInfoForReturnAddress(returnAddress,
				moduleName, moduleBase, symbolName, symbolAddress)) {
			Delete<CPPSymbol> umsymbol(unmangleCPPSymbol_native(symbolName));
			if(*umsymbol)
				writeSymbol(**umsymbol);
			else
				writeFrame(Transcode::utf8ToBMP(symbolName));
			writeFrame(str_blankLeftSquare);
			highlightMemoryAddress();
			writeFrame(Transcode::utf8ToBMP(StackTrace::formatMemoryAddress(symbolAddress)));
			endHighlighting();
			writeFrame(str_blankPlusBlank);
			highlightMemoryAddress();
			writeFrame(Transcode::utf8ToBMP(StackTrace::formatMemoryAddress(
					StackTrace::addressDifference(returnAddress, symbolAddress))));
			endHighlighting();
			writeFrame(str_rightSquare);
			if(!moduleName.empty()) {
				beginFrameModule(static_cast<MemorySize>(rastr.length()));
				writeBeforeModuleName();
				highlightModuleName();
				writeFrameModule(Transcode::utf8ToBMP(moduleName));
				endHighlighting();
				writeAfterModuleName();
				writeFrameModule(str_blankLeftSquare);
				highlightMemoryAddress();
				writeFrameModule(Transcode::utf8ToBMP(StackTrace::formatMemoryAddress(moduleBase)));
				endHighlighting();
				writeFrameModule(str_blankPlusBlank);
				highlightMemoryAddress();
				writeFrameModule(Transcode::utf8ToBMP(StackTrace::formatMemoryAddress(
						StackTrace::addressDifference(returnAddress, moduleBase))));
				endHighlighting();
				writeFrameModule(str_rightSquare);
				endFrameModule();
			}
		}
		else {
			highlightUnresolvedSymbol();
			writeUnresolvedSymbol();
			endHighlighting();
		}
		endFrame();
	}

	void StackTraceSinkBase16::writeFrameNotReconstructible() {
		writeFrame(StackTraceSinkBase16::msg_frameNotReconstructible);
	}

	void StackTraceSinkBase16::writeBeforeModuleName() {
		writeFrameModule(StackTraceSinkBase16::msg_beforeModuleName);
	}

	void StackTraceSinkBase16::writeAfterModuleName() {
		writeFrameModule(StackTraceSinkBase16::msg_afterModuleName);
	}

	void StackTraceSinkBase16::writeUnresolvedSymbol() {
		writeFrame(StackTraceSinkBase16::msg_unresolvedSymbol);
	}

	void StackTraceSinkBase16::endStackTrace(MemorySize frameCount) {
		beginFrame();
		highlightBottomOfUnwinding();
		writeBottomOfUnwinding(frameCount);
		endHighlighting();
		endFrame();
	}

	void StackTraceSinkBase16::writeBottomOfUnwinding(MemorySize frameCount) {
		writeFrame(StackTraceSinkBase16::msg_startBottomOfUnwinding);
		writeFrame(DefaultIntegerFormatter<Char16>::formatInteger<
			MemorySize,
			DefaultFormattingRendition<Char16>
		>(frameCount, FormattingOptions<Char16>(DefaultFormattingOptionStringEmitter<Char16>::instance)));
		writeFrame(StackTraceSinkBase16::msg_endBottomOfUnwinding);
	}

}}
