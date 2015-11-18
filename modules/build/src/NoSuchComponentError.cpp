#include "NoSuchComponentError.hpp"

using std::string;

namespace redengine {
namespace build {

	NoSuchComponentError::NoSuchComponentError(const string& component) : component(component) {}

	NoSuchComponentError::NoSuchComponentError(const NoSuchComponentError& error)
			: BuildError(error), component(error.component) {}

	NoSuchComponentError::~NoSuchComponentError() {}

	REDSTRAIN_DEFINE_ERROR(NoSuchComponentError) {
		out << "No such component: " << component;
	}

}}
