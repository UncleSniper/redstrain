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

	  public:
		class REDSTRAIN_BUILD_API ComponentNameIterator {

		  private:
			Components::const_iterator iterator;

		  public:
			ComponentNameIterator();
			ComponentNameIterator(const Components::const_iterator&);
			ComponentNameIterator(const ComponentNameIterator&);

			const std::string& operator*() const;
			const std::string* operator->() const;
			ComponentNameIterator& operator++();
			ComponentNameIterator operator++(int);
			bool operator==(const ComponentNameIterator&) const;
			bool operator!=(const ComponentNameIterator&) const;
			ComponentNameIterator& operator=(const ComponentNameIterator&);

		};

	  private:
		const std::string name, baseDirectory;
		Components components;

	  public:
		Project(const std::string&, const std::string&);
		Project(const Project&);
		~Project();

		inline const std::string& getName() const {
			return name;
		}

		inline const std::string& getBaseDirectory() const {
			return baseDirectory;
		}

		bool addComponent(Component*);
		bool removeComponent(Component*);
		void clearComponents();
		Component* getComponent(const std::string&) const;
		void getComponents(ComponentNameIterator&, ComponentNameIterator&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_PROJECT_HPP */
