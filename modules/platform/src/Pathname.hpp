#ifndef REDSTRAIN_MOD_PLATFORM_PATHNAME_HPP
#define REDSTRAIN_MOD_PLATFORM_PATHNAME_HPP

#include <string>

#include "api.hpp"
#include "platform.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Pathname {

	  public:
		enum NamingMode {
			NOMINAL,
			LOGICAL
		};

	  public:
		static const char *const SEPARATOR;
		static const char *const THIS_DIRECTORY;
		static const char *const PARENT_DIRECTORY;

	  public:
		static bool isAbsolute(const std::string&);
		static bool isRooted(const std::string&);
		static void splitRoot(const std::string&, std::string&, std::string&);
		static std::string join(const std::string&, const std::string&);
		static std::string dirname(const std::string&, NamingMode = NOMINAL);
		static std::string basename(const std::string&);
		static std::string rootOf(const std::string&);
		static std::string pathOf(const std::string&);
		static std::string tidy(const std::string&);
		static std::string getWorkingDirectory();
		static std::string canonicalizeSeparators(const std::string&);
		static std::string normalizeSeparators(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_PATHNAME_HPP */
