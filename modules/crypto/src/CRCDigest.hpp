#ifndef REDSTRAIN_MOD_CRYPTO_CRCDIGEST_HPP
#define REDSTRAIN_MOD_CRYPTO_CRCDIGEST_HPP

#include <stdint.h>

#include "DigestAlgorithm.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API CRCDigest : public DigestAlgorithm {

	  public:
		enum Flags {
			REFLECT_INPUT_BYTES = 001,
			REFLECT_DIGEST      = 002
		};

		enum Polynomial {
			CRC_16_AUG_CCITT,
			CRC_16_CCITT_KERMIT,
			CRC_16_IBM,
			CRC_16_XMODEM,
			CRC_32,
			CRC_32_BZIP2,
			CRC_32_CASTAGNOLI,
			CRC_32_MPEG2,
			CRC_32_POSIX,
			CRC_64_XZ
		};

	  protected:
		const util::MemorySize order;

	  private:
		const uint64_t divisor;
		uint64_t input, initialValue, finalXOR;
		int flags;

	  public:
		CRCDigest(util::MemorySize, uint64_t, int = 0);
		CRCDigest(Polynomial);
		CRCDigest(const CRCDigest&);

		inline unsigned getPolynomialOrder() const {
			return order;
		}

		inline uint64_t getDivisor() const {
			return divisor;
		}

		inline uint64_t getInitialValue() const {
			return initialValue;
		}

		void setInitialValue(uint64_t);

		inline uint64_t getFinalXOR() const {
			return finalXOR;
		}

		inline void setFinalXOR(uint64_t newFinalXOR) {
			finalXOR = newFinalXOR;
		}

		inline int getFlags() const {
			return flags;
		}

		inline void setFlags(int newFlags) {
			flags = newFlags;
		}

		inline bool hasFlags(int mask) {
			return (flags & mask) == mask;
		}

		uint64_t getRemainder();

		virtual util::MemorySize getKeySize() const;
		virtual void reset();
		virtual void digestMessage(const char*, util::MemorySize);
		virtual void getDigest(char*);

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_CRCDIGEST_HPP */
