#include <redstrain/platform/Pathname.hpp>

#include "DirectoryComponentFactory.hpp"

using std::string;
using redengine::platform::Pathname;

namespace redengine {
namespace build {

	DirectoryComponentFactory::DirectoryComponentFactory() {}

	DirectoryComponentFactory::DirectoryComponentFactory(const DirectoryComponentFactory& factory)
			: ComponentFactory(factory) {}

	Component* DirectoryComponentFactory::newComponent(const Project&, const string& baseDirectory,
			Component::Type type) {
		return new Component(type, Pathname::basename(Pathname::tidy(baseDirectory)), baseDirectory);
	}

}}
