#ifndef REDSTRAIN_MOD_CRYPTO_CIPHERALGORITHM_HPP
#define REDSTRAIN_MOD_CRYPTO_CIPHERALGORITHM_HPP

#include "Algorithm.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CipherAlgorithm : Algorithm {

	  public:
		enum Direction {
			ENCRYPT,
			DECRYPT
		};

	  public:
		CipherAlgorithm();
		CipherAlgorithm(const CipherAlgorithm&);

		virtual util::MemorySize getBlockSize() const = 0;
		virtual Direction getDirection() const = 0;
		virtual void setKey(Direction, const char*, const char*) = 0;
		virtual util::MemorySize transformMessage(const char*, util::MemorySize, char*) = 0;
		virtual util::MemorySize endMessage(char*) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CIPHERALGORITHM_HPP */
