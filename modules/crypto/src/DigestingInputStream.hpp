#ifndef REDSTRAIN_MOD_CRYPTO_DIGESTINGINPUTSTREAM_HPP
#define REDSTRAIN_MOD_CRYPTO_DIGESTINGINPUTSTREAM_HPP

#include <redstrain/io/ProxyInputStream.hpp>

namespace redengine {
namespace crypto {

	class DigestAlgorithm;

	class REDSTRAIN_CRYPTO_API DigestingInputStream : public io::ProxyInputStream<char> {

	  private:
		DigestAlgorithm& algorithm;

	  protected:
		virtual util::MemorySize readBlock(char*, util::MemorySize);

	  public:
		DigestingInputStream(DigestAlgorithm&, io::InputStream<char>&);
		DigestingInputStream(const DigestingInputStream&);

		inline DigestAlgorithm& getDigestAlgorithm() {
			return algorithm;
		}

		inline const DigestAlgorithm& getDigestAlgorithm() const {
			return algorithm;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_DIGESTINGINPUTSTREAM_HPP */
