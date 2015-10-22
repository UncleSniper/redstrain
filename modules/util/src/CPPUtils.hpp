#ifndef REDSTRAIN_MOD_UTIL_CPPUTILS_HPP
#define REDSTRAIN_MOD_UTIL_CPPUTILS_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace util {

	class REDSTRAIN_UTIL_API CPPUtils {

	  private:
		static std::string escapeChar(char, bool, char);

	  public:
		static std::string escapeChar(char, bool);
		static std::string escapeString(const std::string&, bool);
		static std::string slugifySymbol(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_CPPUTILS_HPP */
