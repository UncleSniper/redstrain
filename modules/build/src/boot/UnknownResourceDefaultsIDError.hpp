#ifndef REDSTRAIN_MOD_BUILD_BOOT_UNKNOWNRESOURCEDEFAULTSIDERROR_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_UNKNOWNRESOURCEDEFAULTSIDERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "Resources.hpp"

namespace redengine {
namespace build {
namespace boot {

	class REDSTRAIN_BUILD_API UnknownResourceDefaultsIDError : public error::IllegalArgumentError {

	  private:
		const Resources::Defaults defaultsID;

	  public:
		UnknownResourceDefaultsIDError(Resources::Defaults);
		UnknownResourceDefaultsIDError(const UnknownResourceDefaultsIDError&);

		inline Resources::Defaults getDefaultsID() const {
			return defaultsID;
		}

		REDSTRAIN_DECLARE_ERROR(UnknownResourceDefaultsIDError)

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_UNKNOWNRESOURCEDEFAULTSIDERROR_HPP */
