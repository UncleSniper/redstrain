#ifndef REDSTRAIN_MOD_BUILD_BUILDUI_HPP
#define REDSTRAIN_MOD_BUILD_BUILDUI_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace build {

	class Action;

	class REDSTRAIN_BUILD_API BuildUI {

	  public:
		class REDSTRAIN_BUILD_API ActionDescriptor {

		  private:
			std::string componentType, componentName;
			std::string actionName, source, target;

		  public:
			ActionDescriptor(const std::string&, const std::string&,
					const std::string&, const std::string&, const std::string&);
			ActionDescriptor(const ActionDescriptor&);

			inline const std::string& getComponentType() const {
				return componentType;
			}

			void setComponentType(const std::string&);

			inline const std::string& getComponentName() const {
				return componentName;
			}

			void setComponentName(const std::string&);

			inline const std::string& getActionName() const {
				return actionName;
			}

			void setActionName(const std::string&);

			inline const std::string& getSource() const {
				return source;
			}

			void setSource(const std::string&);

			inline const std::string& getTarget() const {
				return target;
			}

			void setTarget(const std::string&);

		};

	  public:
		BuildUI();
		BuildUI(const BuildUI&);
		virtual ~BuildUI();

		virtual void willPerformAction(const Action&) = 0;
		virtual void wouldPerformAction(const Action&) = 0;
		virtual void willPerformAction(const ActionDescriptor&) = 0;
		virtual void wouldPerformAction(const ActionDescriptor&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDUI_HPP */
