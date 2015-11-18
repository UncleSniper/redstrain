#ifndef REDSTRAIN_MOD_BUILD_UNRECOGNIZEDARCHITECTURENAME_HPP
#define REDSTRAIN_MOD_BUILD_UNRECOGNIZEDARCHITECTURENAME_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API UnrecognizedArchitectureNameError : public error::IllegalArgumentError {

	  private:
		const std::string architecture;

	  public:
		UnrecognizedArchitectureNameError(const std::string&);
		UnrecognizedArchitectureNameError(const UnrecognizedArchitectureNameError&);
		virtual ~UnrecognizedArchitectureNameError();

		inline const std::string& getArchitectureName() const {
			return architecture;
		}

		REDSTRAIN_DECLARE_ERROR(UnrecognizedArchitectureNameError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_UNRECOGNIZEDARCHITECTURENAME_HPP */
