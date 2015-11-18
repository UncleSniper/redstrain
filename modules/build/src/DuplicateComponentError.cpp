#include "DuplicateComponentError.hpp"

using std::string;

namespace redengine {
namespace build {

	DuplicateComponentError::DuplicateComponentError(const string& component) : component(component) {}

	DuplicateComponentError::DuplicateComponentError(const DuplicateComponentError& error)
			: BuildError(error), component(error.component) {}

	DuplicateComponentError::~DuplicateComponentError() {}

	REDSTRAIN_DEFINE_ERROR(DuplicateComponentError) {
		out << "A component named '" << component << "' already exists in the project";
	}

}}
