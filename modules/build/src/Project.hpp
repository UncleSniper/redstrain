#ifndef REDSTRAIN_MOD_BUILD_PROJECT_HPP
#define REDSTRAIN_MOD_BUILD_PROJECT_HPP

#include <map>
#include <string>

#include "api.hpp"

namespace redengine {
namespace build {

	class Component;

	class REDSTRAIN_BUILD_API Project {

	  private:
		typedef std::map<std::string, Component*> Components;
		typedef Components::iterator ComponentIterator;

	  private:
		Components components;

	  public:
		Project();
		Project(const Project&);
		~Project();

		bool addComponent(Component*);
		bool removeComponent(Component*);
		void clearComponents();
		Component* getComponent(const std::string&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_PROJECT_HPP */
