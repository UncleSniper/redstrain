#ifndef REDSTRAIN_MOD_CRYPTO_ILLEGALPADDINGCODEERROR_HPP
#define REDSTRAIN_MOD_CRYPTO_ILLEGALPADDINGCODEERROR_HPP

#include "CryptographyError.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API IllegalPaddingCodeError : public CryptographyError {

	  public:
		IllegalPaddingCodeError();
		IllegalPaddingCodeError(const IllegalPaddingCodeError&);

		REDSTRAIN_DECLARE_ERROR(IllegalPaddingCodeError)

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_ILLEGALPADDINGCODEERROR_HPP */
