#ifndef REDSTRAIN_MOD_BUILD_COMPILER_HPP
#define REDSTRAIN_MOD_BUILD_COMPILER_HPP

#include "Compilation.hpp"
#include "Architecture.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Compiler {

	  private:
		Architecture architecture;

	  public:
		Compiler(Architecture = REDSTRAIN_BUILD_DEFAULT_ARCH);
		Compiler(const Compiler&);
		virtual ~Compiler();

		virtual Compilation* newCompilation(const std::string&, Compilation::CompileMode) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILER_HPP */
