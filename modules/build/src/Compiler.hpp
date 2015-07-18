#ifndef REDSTRAIN_MOD_BUILD_COMPILER_HPP
#define REDSTRAIN_MOD_BUILD_COMPILER_HPP

#include <redstrain/redmond/constants.hpp>

#include "Compilation.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Compiler {

	  private:
		redmond::Architecture architecture;

	  protected:
		inline void setTargetArchitecture(redmond::Architecture architecture) {
			this->architecture = architecture;
		}

	  public:
		Compiler(redmond::Architecture = REDSTRAIN_BUILD_DEFAULT_ARCH);
		Compiler(const Compiler&);
		virtual ~Compiler();

		inline redmond::Architecture getTargetArchitecture() {
			return architecture;
		}

		virtual Compilation* newCompilation(const std::string&, Compilation::CompileMode) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILER_HPP */
