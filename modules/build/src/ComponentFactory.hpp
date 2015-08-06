#ifndef REDSTRAIN_MOD_BUILD_COMPONENTFACTORY_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTFACTORY_HPP

#include "Component.hpp"

namespace redengine {
namespace build {

	class Project;

	class REDSTRAIN_BUILD_API ComponentFactory {

	  public:
		ComponentFactory();
		ComponentFactory(const ComponentFactory&);
		virtual ~ComponentFactory();

		virtual Component* newComponent(const Project&, const std::string&, Component::Type) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENTFACTORY_HPP */
