#ifndef REDSTRAIN_MOD_BUILD_EMPTYLINKERCONFIGURATION_HPP
#define REDSTRAIN_MOD_BUILD_EMPTYLINKERCONFIGURATION_HPP

#include "LinkerConfiguration.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API EmptyLinkerConfiguration : public LinkerConfiguration {

	  public:
		EmptyLinkerConfiguration();
		EmptyLinkerConfiguration(const EmptyLinkerConfiguration&);

		virtual void applyConfiguration(Linkage&);

	  public:
		static EmptyLinkerConfiguration instance;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_EMPTYLINKERCONFIGURATION_HPP */
