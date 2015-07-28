#ifndef REDSTRAIN_MOD_BUILD_ACTION_HPP
#define REDSTRAIN_MOD_BUILD_ACTION_HPP

#include <string>
#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class BuildUI;
	class BuildContext;

	class REDSTRAIN_BUILD_API Action : public util::ReferenceCounted {

		std::string componentType, componentName, componentBase;

	  public:
		Action();
		Action(const Action&);

		inline const std::string& getComponentType() const {
			return componentType;
		}

		inline const std::string& getComponentName() const {
			return componentName;
		}

		inline const std::string& getComponentBaseDirectory() const {
			return componentBase;
		}

		void setComponentType(const std::string&);
		void setComponentName(const std::string&);
		void setComponentBaseDirectory(const std::string&);

		virtual void perform(BuildContext&) = 0;
		virtual void wouldPerform(BuildContext&) = 0;
		virtual void notifyUIWillPerform(BuildUI&) const = 0;
		virtual void notifyUIWouldPerform(BuildUI&) const = 0;
		virtual void slateRebuilds(BuildContext&);

		virtual void dumpAction(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ACTION_HPP */
