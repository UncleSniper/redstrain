#ifndef REDSTRAIN_MOD_BUILD_PROJECT_HPP
#define REDSTRAIN_MOD_BUILD_PROJECT_HPP

#include <string>
#include <redstrain/util/MapKeyIterator.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Component;

	class REDSTRAIN_BUILD_API Project {

	  public:
		enum REDSTRAIN_BUILD_API ComponentCategory {
			LIBRARY,
			EXECUTABLE
		};

		class REDSTRAIN_BUILD_API ComponentReference {

		  private:
			const ComponentCategory category;
			const std::string name;

		  public:
			ComponentReference(ComponentCategory, const std::string&);
			ComponentReference(const ComponentReference&);

			inline ComponentCategory getCategory() const {
				return category;
			}

			inline const std::string& getName() const {
				return name;
			}

			bool operator==(const ComponentReference&) const;
			bool operator!=(const ComponentReference&) const;
			bool operator<(const ComponentReference&) const;

		};

	  private:
		typedef std::map<ComponentReference, Component*> Components;
		typedef Components::iterator ComponentIterator;
		typedef Components::const_iterator ConstComponentIterator;

	  public:
		typedef util::MapKeyIterator<ComponentReference, Component*> ComponentNameIterator;

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

		bool addComponent(Component&);
		bool removeComponent(Component&);
		void clearComponents();
		Component* getComponent(ComponentCategory, const std::string&) const;
		Component* getComponent(const ComponentReference&) const;
		void getComponents(ComponentNameIterator&, ComponentNameIterator&) const;

		std::string::size_type getMaximalComponentNameWidth() const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_PROJECT_HPP */
