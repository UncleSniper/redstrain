#include <sstream>
#include <cstdlib>
#include <redstrain/util/Delete.hpp>

#include "Error.hpp"
#include "StackTrace.hpp"
#include "StdOStreamStackTraceSink.hpp"
#include "StdOStreamStackTraceIndenter.hpp"
#include "tweaks.hpp"

using std::endl;
using std::string;
using std::ostream;
using std::stringstream;
using redengine::util::Delete;
using redengine::util::MemorySize;
using redengine::redmond::StackUnwinder;
using redengine::redmond::getStackUnwinder;

namespace redengine {
namespace error {

	Error::Error() : refCount(1u), stackTrace(NULL) {
		Delete<StackUnwinder> unwinder(getStackUnwinder());
		if(*unwinder)
			stackTrace = new StackTrace(**unwinder);
	}

	Error::Error(const Error& error) : refCount(1u), stackTrace(error.stackTrace) {
		if(stackTrace)
			stackTrace->ref();
	}

	Error::~Error() {
		if(stackTrace)
			stackTrace->unref();
	}

	void Error::ref() {
		++refCount;
	}

	void Error::unref() {
		if(!--refCount)
			delete this;
	}

	static inline void* addrDiff(void* a, void* b) {
		return reinterpret_cast<void*>(reinterpret_cast<char*>(a) - reinterpret_cast<char*>(b));
	}

	static const unsigned ADDRESS_WIDTH = static_cast<unsigned>(sizeof(void*)) * 2u + 2u;
	static const char *const ADDRESS_INDENT = "                                  ";

	void Error::printStackTrace(StackTraceSink& sink) const {
		if(stackTrace)
			stackTrace->printTo(sink);
		else
			sink.noStackTraceAvailable();
	}

	void Error::printStackTrace(ostream& out) const {
		StdOStreamStackTraceIndenter indenter(out);
		StdOStreamStackTraceSink sink(out, indenter);
		printStackTrace(sink);
	}

	string Error::getMessage() const {
		stringstream ss;
		printMessage(ss);
		return ss.str();
	}

	const char* Error::getTypename() {
		return "Error";
	}

	bool Error::shouldPrintStackTraces() {
		enum Result {
			YES,
			NO,
			UNKNOWN
		};
		static Result result = UNKNOWN;
		if(result == UNKNOWN)
			result = getenv(REDSTRAIN_ERROR_PRINT_STACK_TRACES_ENVVAR) ? YES : NO;
		return result != NO;
	}

}}
