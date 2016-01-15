#ifndef REDSTRAIN_MOD_CRYPTO_CRCORDEROUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_CRYPTO_CRCORDEROUTOFBOUNDSERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>
#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CRCOrderOutOfBoundsError : public error::IllegalArgumentError {

	  private:
		const util::MemorySize order;

	  public:
		CRCOrderOutOfBoundsError(util::MemorySize);
		CRCOrderOutOfBoundsError(const CRCOrderOutOfBoundsError&);

		inline util::MemorySize getOrder() const {
			return order;
		}

		REDSTRAIN_DECLARE_ERROR(CRCOrderOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CRCORDEROUTOFBOUNDSERROR_HPP */
