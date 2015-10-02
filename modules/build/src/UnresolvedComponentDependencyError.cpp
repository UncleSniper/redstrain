#include "UnresolvedComponentDependencyError.hpp"

using std::string;

namespace redengine {
namespace build {

	UnresolvedComponentDependencyError::UnresolvedComponentDependencyError(const string& outer, const string& inner)
			: NoSuchComponentError(inner), outer(outer) {}

	UnresolvedComponentDependencyError::UnresolvedComponentDependencyError(const UnresolvedComponentDependencyError&
			error) : NoSuchComponentError(error), outer(error.outer) {}

	REDSTRAIN_DEFINE_ERROR(UnresolvedComponentDependencyError) {
		out << "Component '" << outer << "' depends on unknown component '" << getComponentName() << "'";
	}

}}
