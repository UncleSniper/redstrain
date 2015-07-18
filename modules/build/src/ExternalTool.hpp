#ifndef REDSTRAIN_MOD_BUILD_EXTERNALTOOL_HPP
#define REDSTRAIN_MOD_BUILD_EXTERNALTOOL_HPP

#include <string>
#include <redstrain/platform/ProcessBuilder.hpp>

#include "Invocation.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ExternalTool {

	  public:
		class REDSTRAIN_BUILD_API ExternalInvocation : public virtual Invocation {

		  protected:
			platform::ProcessBuilder command;

		  protected:
			void checkStatus(int) const;

		  public:
			ExternalInvocation(const std::string&);
			ExternalInvocation(const ExternalInvocation&);

			inline platform::ProcessBuilder& getCommand() {
				return command;
			}

			inline const platform::ProcessBuilder& getCommand() const {
				return command;
			}

			virtual void invoke();

		};

	  private:
		std::string executable;

	  public:
		ExternalTool(const std::string&);
		ExternalTool(const ExternalTool&);
		virtual ~ExternalTool();

		inline const std::string& getExecutable() const {
			return executable;
		}

		void setExecutable(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_EXTERNALTOOL_HPP */
