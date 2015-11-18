#ifndef REDSTRAIN_MOD_IO_NUMBERADDRESSSPACEERROR_HPP
#define REDSTRAIN_MOD_IO_NUMBERADDRESSSPACEERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API NumberAddressSpaceError : public error::IllegalArgumentError {

	  private:
		const std::string rendition;

	  public:
		NumberAddressSpaceError(const std::string&);
		NumberAddressSpaceError(const NumberAddressSpaceError&);

		inline const std::string& getOffendingRendition() const {
			return rendition;
		}

		REDSTRAIN_DECLARE_ERROR(NumberAddressSpaceError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_NUMBERADDRESSSPACEERROR_HPP */
