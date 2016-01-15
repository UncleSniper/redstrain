#ifndef REDSTRAIN_MOD_CRYPTO_CIPHERINGOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_CRYPTO_CIPHERINGOUTPUTSTREAM_HPP

#include <redstrain/io/ProxyOutputStream.hpp>

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CipheringOutputStream : public io::ProxyOutputStream<char> {

	  private:
		CipherAlgorithm& algorithm;
		char* buffer;
		const util::MemorySize blockSize;
		util::MemorySize bufferSize, bufferFill, bufferOffset, pendingCount;

	  private:
		void flushBuffer();

	  protected:
		CipheringOutputStream(const CipheringOutputStream&);

		virtual void writeBlock(const char*, util::MemorySize);

	  public:
		CipheringOutputStream(CipherAlgorithm&, io::OutputStream<char>&);
		virtual ~CipheringOutputStream();

		inline CipherAlgorithm& getCipherAlgorithm() {
			return algorithm;
		}

		inline const CipherAlgorithm& getCipherAlgorithm() const {
			return algorithm;
		}

		void endMessage();

		virtual void close();

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CIPHERINGOUTPUTSTREAM_HPP */
