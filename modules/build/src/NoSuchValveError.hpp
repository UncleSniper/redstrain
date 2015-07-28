#ifndef REDSTRAIN_MOD_BUILD_NOSUCHVALVEERROR_HPP
#define REDSTRAIN_MOD_BUILD_NOSUCHVALVEERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API NoSuchValveError : public BuildError {

	  private:
		const std::string valve;

	  public:
		NoSuchValveError(const std::string&);
		NoSuchValveError(const NoSuchValveError&);

		inline const std::string& getValveName() const {
			return valve;
		}

		REDSTRAIN_DECLARE_ERROR(NoSuchValveError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_NOSUCHVALVEERROR_HPP */
