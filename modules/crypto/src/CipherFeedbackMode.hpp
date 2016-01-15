#ifndef REDSTRAIN_MOD_CRYPTO_CIPHERFEEDBACKMODE_HPP
#define REDSTRAIN_MOD_CRYPTO_CIPHERFEEDBACKMODE_HPP

#include "BufferingCipherMode.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CipherFeedbackMode : public BufferingCipherMode {

	  public:
		CipherFeedbackMode();
		CipherFeedbackMode(const CipherFeedbackMode&);

		virtual const char* preprocessBlock(CipherAlgorithm::Direction&,
				const char*, char*, util::MemorySize);
		virtual const char* postprocessBlock(CipherAlgorithm::Direction,
				const char*, char*, char*, util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CIPHERFEEDBACKMODE_HPP */
