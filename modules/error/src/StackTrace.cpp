#include <redstrain/redmond/environment.hpp>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* !_GNU_SOURCE */
#include <cstring>

#include "StackTrace.hpp"
#include "StackTraceSink.hpp"
#include "FrameIndexOutOfBoundsError.hpp"

#ifdef REDSTRAIN_ENV_OS_LINUX
#include <dlfcn.h>
#endif /* OS-specific includes */

using std::string;
using redengine::util::MemorySize;
using redengine::util::IntegerPointer;
using redengine::util::ReferenceCounted;
using redengine::redmond::StackUnwinder;

namespace redengine {
namespace error {

	StackTrace::StackTrace(StackUnwinder& unwinder) : frameCount(static_cast<MemorySize>(unwinder.getFrameDepth())),
			returnAddresses(frameCount ? new void*[frameCount] : NULL) {
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < frameCount; ++u)
			returnAddresses[u] = unwinder.getNextReturnAddress();
	}

	StackTrace::StackTrace(const StackTrace& trace) : ReferenceCounted(trace), frameCount(trace.frameCount),
			returnAddresses(frameCount ? new void*[frameCount] : NULL) {
		if(frameCount)
			memcpy(returnAddresses, trace.returnAddresses, static_cast<size_t>(frameCount) * sizeof(void*));
	}

	StackTrace::~StackTrace() {
		if(frameCount)
			delete[] returnAddresses;
	}

	void* StackTrace::getReturnAddressOfFrame(MemorySize frame) const {
		if(frame >= frameCount)
			throw FrameIndexOutOfBoundsError(frame);
		return returnAddresses[frame];
	}

	void StackTrace::printTo(StackTraceSink& sink) const {
		sink.beginStackTrace(frameCount);
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < frameCount; ++u)
			sink.putStackFrame(returnAddresses[u], u, frameCount);
		sink.endStackTrace(frameCount);
	}

#ifdef REDSTRAIN_ENV_OS_LINUX

	bool StackTrace::getSymbolInfoForReturnAddress(void* returnAddress, string& moduleName, void*& moduleBase,
			string& symbolName, void*& symbolAddress) {
		Dl_info info;
		if(!dladdr(returnAddress, &info))
			return false;
		if(!info.dli_sname)
			return false;
		if(info.dli_fname) {
			moduleName = info.dli_fname;
			moduleBase = info.dli_fbase;
		}
		else {
			moduleName.clear();
			moduleBase = NULL;
		}
		symbolName = info.dli_sname;
		symbolAddress = info.dli_saddr;
		return true;
	}

#else /* unsupported OS */

	bool StackTrace::getSymbolInfoForReturnAddress(void*, string&, void*&, string&, void*&) {
		return false;
	}

#endif /* OSes */

	static const unsigned ADDRESS_WIDTH = static_cast<unsigned>(sizeof(void*)) * 2u + 2u;

	string StackTrace::formatMemoryAddress(void* address) {
		char buffer[ADDRESS_WIDTH];
		IntegerPointer ia = reinterpret_cast<IntegerPointer>(address);
		unsigned u;
		char* insert = buffer + ADDRESS_WIDTH;
		for(u = 0u; u < ADDRESS_WIDTH; ++u) {
			*--insert = "0123456789ABCDEF"[ia % static_cast<IntegerPointer>(16u)];
			ia /= static_cast<IntegerPointer>(16u);
		}
		buffer[1] = 'x';
		return string(buffer, static_cast<string::size_type>(ADDRESS_WIDTH));
	}

}}
