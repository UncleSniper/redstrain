#ifndef REDSTRAIN_MOD_BUILD_ACTION_HPP
#define REDSTRAIN_MOD_BUILD_ACTION_HPP

#include <string>
#include <redstrain/util/ReferenceCounted.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamtypes.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "api.hpp"

namespace redengine {
namespace build {

	class BuildUI;
	class BuildContext;

	class REDSTRAIN_BUILD_API Action : public util::ReferenceCounted {

		std::string componentType, componentName;

	  public:
		Action();
		Action(const Action&);

		inline const std::string& getComponentType() const {
			return componentType;
		}

		inline const std::string& getComponentName() const {
			return componentName;
		}

		void setComponentType(const std::string&);
		void setComponentName(const std::string&);

		virtual void perform(BuildContext&) = 0;
		virtual void wouldPerform(BuildContext&) = 0;
		virtual void notifyUIWillPerform(BuildUI&) const = 0;
		virtual void notifyUIWouldPerform(BuildUI&) const = 0;

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpAction(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ACTION_HPP */
