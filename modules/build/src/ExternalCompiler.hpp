#ifndef REDSTRAIN_MOD_BUILD_EXTERNALCOMPILER_HPP
#define REDSTRAIN_MOD_BUILD_EXTERNALCOMPILER_HPP

#include <redstrain/platform/ProcessBuilder.hpp>

#include "Compiler.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ExternalCompiler : public Compiler {

	  public:
		class REDSTRAIN_BUILD_API ExternalCompilation : public Compilation {

		  protected:
			platform::ProcessBuilder command;

		  public:
			ExternalCompilation(const std::string&, const std::string&, CompileMode);
			ExternalCompilation(const ExternalCompilation&);

			inline platform::ProcessBuilder& getCommand() {
				return command;
			}

			inline const platform::ProcessBuilder& getCommand() const {
				return command;
			}

		};

	  private:
		std::string executable;

	  public:
		ExternalCompiler(const std::string&, Architecture);
		ExternalCompiler(const ExternalCompiler&);

		inline const std::string& getExecutable() const {
			return executable;
		}

		void setExecutable(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_EXTERNALCOMPILER_HPP */
