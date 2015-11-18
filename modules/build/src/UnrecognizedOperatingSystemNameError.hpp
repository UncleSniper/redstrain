#ifndef REDSTRAIN_MOD_BUILD_UNRECOGNIZEDOPERATINGSYSTEMNAMEERROR_HPP
#define REDSTRAIN_MOD_BUILD_UNRECOGNIZEDOPERATINGSYSTEMNAMEERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API UnrecognizedOperatingSystemNameError : public error::IllegalArgumentError {

	  private:
		const std::string architecture;

	  public:
		UnrecognizedOperatingSystemNameError(const std::string&);
		UnrecognizedOperatingSystemNameError(const UnrecognizedOperatingSystemNameError&);
		virtual ~UnrecognizedOperatingSystemNameError();

		inline const std::string& getArchitectureName() const {
			return architecture;
		}

		REDSTRAIN_DECLARE_ERROR(UnrecognizedOperatingSystemNameError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_UNRECOGNIZEDOPERATINGSYSTEMNAMEERROR_HPP */
