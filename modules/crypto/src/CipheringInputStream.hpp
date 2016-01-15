#ifndef REDSTRAIN_MOD_CRYPTO_CIPHERINGINPUTSTREAM_HPP
#define REDSTRAIN_MOD_CRYPTO_CIPHERINGINPUTSTREAM_HPP

#include <redstrain/io/ProxyInputStream.hpp>

namespace redengine {
namespace crypto {

	class CipherAlgorithm;

	class REDSTRAIN_CRYPTO_API CipheringInputStream : public io::ProxyInputStream<char> {

	  private:
		CipherAlgorithm& algorithm;
		char* buffer;
		const util::MemorySize blockSize;
		util::MemorySize bufferSize, bufferFill, bufferOffset, pendingCount;

	  private:
		void fillBuffer();

	  protected:
		CipheringInputStream(const CipheringInputStream&);

		virtual util::MemorySize readBlock(char*, util::MemorySize);

	  public:
		CipheringInputStream(CipherAlgorithm&, io::InputStream<char>&);
		virtual ~CipheringInputStream();

		inline CipherAlgorithm& getCipherAlgorithm() {
			return algorithm;
		}

		inline const CipherAlgorithm& getCipherAlgorithm() const {
			return algorithm;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CIPHERINGINPUTSTREAM_HPP */
