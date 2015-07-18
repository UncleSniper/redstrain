#ifndef REDSTRAIN_MOD_BUILD_EXTERNALCOMPILER_HPP
#define REDSTRAIN_MOD_BUILD_EXTERNALCOMPILER_HPP

#include "Compiler.hpp"
#include "ExternalTool.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ExternalCompiler : public virtual ExternalTool, public Compiler {

	  public:
		class REDSTRAIN_BUILD_API ExternalCompilation : public Compilation, public ExternalInvocation {

		  public:
			ExternalCompilation(const std::string&, const std::string&, CompileMode);
			ExternalCompilation(const ExternalCompilation&);

		};

	  public:
		ExternalCompiler(const std::string&, Architecture);
		ExternalCompiler(const ExternalCompiler&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_EXTERNALCOMPILER_HPP */
