#ifndef REDSTRAIN_MOD_BUILD_EMPTYCOMPILERCONFIGURATION_HPP
#define REDSTRAIN_MOD_BUILD_EMPTYCOMPILERCONFIGURATION_HPP

#include "CompilerConfiguration.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API EmptyCompilerConfiguration : public CompilerConfiguration {

	  public:
		EmptyCompilerConfiguration();
		EmptyCompilerConfiguration(const EmptyCompilerConfiguration&);

		virtual void applyConfiguration(Compilation&);

	  public:
		static EmptyCompilerConfiguration instance;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_EMPTYCOMPILERCONFIGURATION_HPP */
