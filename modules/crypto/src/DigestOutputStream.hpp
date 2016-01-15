#ifndef REDSTRAIN_MOD_CRYPTO_DIGESTOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_CRYPTO_DIGESTOUTPUTSTREAM_HPP

#include <redstrain/io/OutputStream.hpp>

namespace redengine {
namespace crypto {

	class DigestAlgorithm;

	class REDSTRAIN_CRYPTO_API DigestOutputStream : public io::OutputStream<char> {

	  private:
		DigestAlgorithm& algorithm;

	  protected:
		virtual void writeBlock(const char*, util::MemorySize);

	  public:
		DigestOutputStream(DigestAlgorithm&);
		DigestOutputStream(const DigestOutputStream&);

		inline DigestAlgorithm& getDigestAlgorithm() {
			return algorithm;
		}

		inline const DigestAlgorithm& getDigestAlgorithm() const {
			return algorithm;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_DIGESTOUTPUTSTREAM_HPP */
