#ifndef REDSTRAIN_MOD_BUILD_FLAVOR_HPP
#define REDSTRAIN_MOD_BUILD_FLAVOR_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Flavor {

	  private:
		std::string name;

	  public:
		Flavor(const std::string&);
		Flavor(const Flavor&);

		inline const std::string& getName() const {
			return name;
		}

		Flavor& operator=(const Flavor&);

		static const Flavor SHIPPED;
		static const Flavor CLEAN;
		static const Flavor STATIC;
		static const Flavor DYNAMIC;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FLAVOR_HPP */
