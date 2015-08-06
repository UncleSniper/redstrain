#ifndef REDSTRAIN_MOD_BUILD_COMPILERCONFIGURATION_HPP
#define REDSTRAIN_MOD_BUILD_COMPILERCONFIGURATION_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Compilation;

	class REDSTRAIN_BUILD_API CompilerConfiguration {

	  public:
		CompilerConfiguration();
		CompilerConfiguration(const CompilerConfiguration&);
		virtual ~CompilerConfiguration();

		virtual void applyConfiguration(Compilation&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILERCONFIGURATION_HPP */
