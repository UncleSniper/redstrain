#include <redstrain/io/streamoperators.hpp>

#include "ComponentUIInfo.hpp"

using std::string;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::indent;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	ComponentUIInfo::ComponentUIInfo() {}

	ComponentUIInfo::ComponentUIInfo(const ComponentUIInfo& info) : componentType(info.componentType),
			componentName(info.componentName), componentBase(info.componentBase) {}

	ComponentUIInfo::~ComponentUIInfo() {}

	void ComponentUIInfo::setComponentType(const string& type) {
		componentType = type;
	}

	void ComponentUIInfo::setComponentName(const string& name) {
		componentName = name;
	}

	void ComponentUIInfo::setComponentBaseDirectory(const string& directory) {
		componentBase = directory;
	}

	void ComponentUIInfo::dumpComponentUIInfoAspects(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "componentType = '" << componentType << '\'' << endln;
		stream << indent << "componentName = '" << componentName << '\'' << endln;
		stream << indent << "componentBase = '" << componentBase << '\'' << endln;
	}

}}
