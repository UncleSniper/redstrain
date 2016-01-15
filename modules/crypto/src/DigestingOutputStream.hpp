#ifndef REDSTRAIN_MOD_CRYPTO_DIGESTINGOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_CRYPTO_DIGESTINGOUTPUTSTREAM_HPP

#include <redstrain/io/ProxyOutputStream.hpp>

namespace redengine {
namespace crypto {

	class DigestAlgorithm;

	class REDSTRAIN_CRYPTO_API DigestingOutputStream : public io::ProxyOutputStream<char> {

	  private:
		DigestAlgorithm& algorithm;

	  protected:
		virtual void writeBlock(const char*, util::MemorySize);

	  public:
		DigestingOutputStream(DigestAlgorithm&, io::OutputStream<char>&);
		DigestingOutputStream(const DigestingOutputStream&);

		inline DigestAlgorithm& getDigestAlgorithm() {
			return algorithm;
		}

		inline const DigestAlgorithm& getDigestAlgorithm() const {
			return algorithm;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_DIGESTINGOUTPUTSTREAM_HPP */
