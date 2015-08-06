#include "Component.hpp"
#include "CyclicDependencyError.hpp"

namespace redengine {
namespace build {

	CyclicDependencyError::CyclicDependencyError(Component& component) : component(&component) {
		component.ref();
	}

	CyclicDependencyError::CyclicDependencyError(const CyclicDependencyError& error)
			: BuildError(error), component(error.component) {
		component->ref();
	}

	CyclicDependencyError::~CyclicDependencyError() {
		component->unref();
	}

	REDSTRAIN_DEFINE_ERROR(CyclicDependencyError) {
		out << "Component '" << component->getName() << "' ultimately depends on itsself";
	}

}}
