#include "BuildUI.hpp"

using std::string;

namespace redengine {
namespace build {

	// ======== ActionDescriptor ========

	BuildUI::ActionDescriptor::ActionDescriptor(const string& componentType, const string& componentName,
			const string& actionName, const string& source, const string& target)
			: componentType(componentType), componentName(componentName), actionName(actionName),
			source(source), target(target) {}

	BuildUI::ActionDescriptor::ActionDescriptor(const ActionDescriptor& descriptor)
			: componentType(descriptor.componentType), componentName(descriptor.componentName),
			actionName(descriptor.actionName), source(descriptor.source), target(descriptor.target) {}

	void BuildUI::ActionDescriptor::setComponentType(const string& type) {
		componentType = type;
	}

	void BuildUI::ActionDescriptor::setComponentName(const string& name) {
		componentName = name;
	}
	void BuildUI::ActionDescriptor::setActionName(const string& name) {
		actionName = name;
	}

	void BuildUI::ActionDescriptor::setSource(const string& source) {
		this->source = source;
	}

	void BuildUI::ActionDescriptor::setTarget(const string& target) {
		this->target = target;
	}

	// ======== BuildUI ========

	BuildUI::BuildUI() {}

	BuildUI::BuildUI(const BuildUI&) {}

	BuildUI::~BuildUI() {}

}}
