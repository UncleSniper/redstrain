#ifndef REDSTRAIN_MOD_BUILD_COMPONENTUIINFO_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTUIINFO_HPP

#include <string>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ComponentUIInfo {

	  private:
		std::string componentType, componentName, componentBase;

	  protected:
		void dumpComponentUIInfoAspects(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	  public:
		ComponentUIInfo();
		ComponentUIInfo(const ComponentUIInfo&);
		virtual ~ComponentUIInfo();

		inline const std::string& getComponentType() const {
			return componentType;
		}

		void setComponentType(const std::string&);

		inline const std::string& getComponentName() const {
			return componentName;
		}

		void setComponentName(const std::string&);

		inline const std::string& getComponentBaseDirectory() const {
			return componentBase;
		}

		void setComponentBaseDirectory(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENTUIINFO_HPP */
