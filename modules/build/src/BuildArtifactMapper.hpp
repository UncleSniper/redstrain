#ifndef REDSTRAIN_MOD_BUILD_BUILDARTIFACTMAPPER_HPP
#define REDSTRAIN_MOD_BUILD_BUILDARTIFACTMAPPER_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace build {

	class Flavor;
	class Language;
	class Component;

	class REDSTRAIN_BUILD_API BuildArtifactMapper {

	  public:
		BuildArtifactMapper();
		BuildArtifactMapper(const BuildArtifactMapper&);
		virtual ~BuildArtifactMapper();

		virtual std::string getTargetFileName(const Component&, const Language&, const Flavor&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDARTIFACTMAPPER_HPP */
