#ifndef REDSTRAIN_MOD_BUILD_NOSUCHGOALERROR_HPP
#define REDSTRAIN_MOD_BUILD_NOSUCHGOALERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API NoSuchGoalError : public BuildError {

	  private:
		const std::string goalName;

	  public:
		NoSuchGoalError(const std::string&);
		NoSuchGoalError(const NoSuchGoalError&);

		inline const std::string& getGoalName() const {
			return goalName;
		}

		REDSTRAIN_DECLARE_ERROR(NoSuchGoalError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_NOSUCHGOALERROR_HPP */
