#ifndef REDSTRAIN_MOD_BUILD_LINKERCONFIGURATION_HPP
#define REDSTRAIN_MOD_BUILD_LINKERCONFIGURATION_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Linkage;

	class REDSTRAIN_BUILD_API LinkerConfiguration {

	  public:
		LinkerConfiguration();
		LinkerConfiguration(const LinkerConfiguration&);
		virtual ~LinkerConfiguration();

		virtual void applyConfiguration(Linkage&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKERCONFIGURATION_HPP */
