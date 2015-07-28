#include "Action.hpp"

using std::string;

namespace redengine {
namespace build {

	Action::Action() {}

	Action::Action(const Action& action) : ReferenceCounted(action) {}

	void Action::setComponentType(const string& type) {
		componentType = type;
	}

	void Action::setComponentName(const string& name) {
		componentName = name;
	}

	void Action::setComponentBaseDirectory(const string& directory) {
		componentBase = directory;
	}

	void Action::slateRebuilds(BuildContext&) {}

}}
