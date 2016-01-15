#ifndef REDSTRAIN_MOD_CRYPTO_DIGESTALGORITHM_HPP
#define REDSTRAIN_MOD_CRYPTO_DIGESTALGORITHM_HPP

#include "Algorithm.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API DigestAlgorithm : public Algorithm {

	  public:
		DigestAlgorithm();
		DigestAlgorithm(const DigestAlgorithm&);

		bool verifyDigest(const char*);

		virtual void digestMessage(const char*, util::MemorySize) = 0;
		virtual void getDigest(char*) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_DIGESTALGORITHM_HPP */
