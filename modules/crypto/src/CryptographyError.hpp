#ifndef REDSTRAIN_MOD_CRYPTO_CRYPTOGRAPHYERROR_HPP
#define REDSTRAIN_MOD_CRYPTO_CRYPTOGRAPHYERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CryptographyError : public error::Error {

	  public:
		CryptographyError();
		CryptographyError(const CryptographyError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(CryptographyError)

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CRYPTOGRAPHYERROR_HPP */
