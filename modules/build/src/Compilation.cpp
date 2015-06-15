#include "Compilation.hpp"

using std::string;

namespace redengine {
namespace build {

	Compilation::Compilation(const string& source, CompileMode mode) : source(source), mode(mode) {}

	Compilation::Compilation(const Compilation& compilation)
			: Invocation(compilation), source(compilation.source), mode(compilation.mode) {}

	void Compilation::setTarget(const string& target) {
		this->target = target;
	}

	void Compilation::clearTarget() {
		target.clear();
	}

}}
