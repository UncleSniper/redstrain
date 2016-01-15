#ifndef REDSTRAIN_MOD_CRYPTO_CIPHERBLOCKCHAININGMODE_HPP
#define REDSTRAIN_MOD_CRYPTO_CIPHERBLOCKCHAININGMODE_HPP

#include "BufferingCipherMode.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CipherBlockChainingMode : public BufferingCipherMode {

	  public:
		CipherBlockChainingMode();
		CipherBlockChainingMode(const CipherBlockChainingMode&);

		virtual const char* preprocessBlock(CipherAlgorithm::Direction&,
				const char*, char*, util::MemorySize);
		virtual const char* postprocessBlock(CipherAlgorithm::Direction,
				const char*, char*, char*, util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CIPHERBLOCKCHAININGMODE_HPP */
