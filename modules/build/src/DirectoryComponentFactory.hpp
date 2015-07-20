#ifndef REDSTRAIN_MOD_BUILD_DIRECTORYCOMPONENTFACTORY_HPP
#define REDSTRAIN_MOD_BUILD_DIRECTORYCOMPONENTFACTORY_HPP

#include "ProjectBuilder.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API DirectoryComponentFactory : public ProjectBuilder::ComponentFactory {

	  public:
		DirectoryComponentFactory();
		DirectoryComponentFactory(const DirectoryComponentFactory&);

		virtual Component* newComponent(const Project&, const std::string&, Component::Type);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DIRECTORYCOMPONENTFACTORY_HPP */
