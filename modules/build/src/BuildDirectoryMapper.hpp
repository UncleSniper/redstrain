#ifndef REDSTRAIN_MOD_BUILD_BUILDDIRECTORYMAPPER_HPP
#define REDSTRAIN_MOD_BUILD_BUILDDIRECTORYMAPPER_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace build {

	class Flavor;
	class Language;
	class Component;

	class REDSTRAIN_BUILD_API BuildDirectoryMapper {

	  public:
		BuildDirectoryMapper();
		BuildDirectoryMapper(const BuildDirectoryMapper&);
		virtual ~BuildDirectoryMapper();

		virtual void getBuildDirectory(const Component&, const Language&, const Flavor&,
				std::string&, std::string&) = 0;
		virtual void getHeaderExposeDirectory(const Component&, const Language&,
				std::string&, std::string&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDDIRECTORYMAPPER_HPP */
