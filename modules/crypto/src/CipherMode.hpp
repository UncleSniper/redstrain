#ifndef REDSTRAIN_MOD_CRYPTO_CIPHERMODE_HPP
#define REDSTRAIN_MOD_CRYPTO_CIPHERMODE_HPP

#include "CipherAlgorithm.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CipherMode {

	  public:
		CipherMode();
		CipherMode(const CipherMode&);
		virtual ~CipherMode();

		virtual const char* preprocessBlock(CipherAlgorithm::Direction&,
				const char*, char*, util::MemorySize) = 0;
		virtual const char* postprocessBlock(CipherAlgorithm::Direction,
				const char*, char*, char*, util::MemorySize) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CIPHERMODE_HPP */
