#ifndef REDSTRAIN_MOD_CRYPTO_ILLEGALINPUTBLOCKSIZEERROR_HPP
#define REDSTRAIN_MOD_CRYPTO_ILLEGALINPUTBLOCKSIZEERROR_HPP

#include <redstrain/util/types.hpp>

#include "CryptographyError.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API IllegalInputBlockSizeError : public CryptographyError {

	  private:
		const util::MemorySize blockSize;

	  public:
		IllegalInputBlockSizeError(util::MemorySize);
		IllegalInputBlockSizeError(const IllegalInputBlockSizeError&);

		inline util::MemorySize getBlockSize() const {
			return blockSize;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalInputBlockSizeError)

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_ILLEGALINPUTBLOCKSIZEERROR_HPP */
