#ifndef REDSTRAIN_MOD_CRYPTO_BUFFERINGCIPHERMODE_HPP
#define REDSTRAIN_MOD_CRYPTO_BUFFERINGCIPHERMODE_HPP

#include "CipherMode.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API BufferingCipherMode : public CipherMode {

	  protected:
		bool firstBlock;
		unsigned bufferSize;
		char* bufferedBlock;

	  protected:
		void allocateBuffer(util::MemorySize, bool);
		void copyBlock(const char*, util::MemorySize);
		void xorWithLastBlock(const char*, char*, util::MemorySize);
		void xorBlock(char*, const char*, util::MemorySize);

	  public:
		BufferingCipherMode();
		BufferingCipherMode(const BufferingCipherMode&);
		virtual ~BufferingCipherMode();

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_BUFFERINGCIPHERMODE_HPP */
