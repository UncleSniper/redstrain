#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEUTILS_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEUTILS_HPP

#include <map>
#include <string>

#include "../api.hpp"

namespace redengine {
namespace build {
namespace boot {

	class REDSTRAIN_BUILD_API XakeUtils {

	  public:
		static std::string subst(const std::string&, const std::map<std::string, std::string>&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEUTILS_HPP */
