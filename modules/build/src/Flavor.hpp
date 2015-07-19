#ifndef REDSTRAIN_MOD_BUILD_FLAVOR_HPP
#define REDSTRAIN_MOD_BUILD_FLAVOR_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Flavor {

	  private:
		const std::string name;

	  public:
		Flavor(const std::string&);
		Flavor(const Flavor&);

		inline const std::string& getName() {
			return name;
		}

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FLAVOR_HPP */
