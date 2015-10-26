#include "Compilation.hpp"

using std::string;

namespace redengine {
namespace build {

	Compilation::Compilation(const string& source, CompileMode mode) : source(source), mode(mode), debug(false) {}

	Compilation::Compilation(const Compilation& compilation)
			: Invocation(compilation), source(compilation.source), mode(compilation.mode),
			debug(compilation.debug) {}

	void Compilation::setTarget(const string& target) {
		this->target = target;
	}

	void Compilation::clearTarget() {
		target.clear();
	}

	const char* Compilation::getCompileModeName(CompileMode mode) {
		switch(mode) {
			#define clamp(c) case c: return #c;
			clamp(FOR_STATIC_EXECUTABLE)
			clamp(FOR_DYNAMIC_EXECUTABLE)
			clamp(FOR_STATIC_LIBRARY)
			clamp(FOR_DYNAMIC_LIBRARY)
			#undef clamp
			default:
				return NULL;
		}
	}

}}
