#include <cstring>
#include <redstrain/platform/Endianness.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "CRCDigest.hpp"
#include "CRCOrderOutOfBoundsError.hpp"

using redengine::util::MemorySize;
using redengine::platform::Endianness;
using redengine::error::IllegalArgumentError;

namespace redengine {
namespace crypto {

	CRCDigest::CRCDigest(MemorySize order, uint64_t divisor, int flags) : order(order), divisor(divisor),
			input(static_cast<uint64_t>(0u)), initialValue(static_cast<uint64_t>(0u)),
			finalXOR(static_cast<uint64_t>(0u)), flags(flags) {
		if(!order || order > static_cast<MemorySize>(64u))
			throw CRCOrderOutOfBoundsError(order);
	}

	static const unsigned orderTable[] = {
		16u, // CRC_16_AUG_CCITT
		16u, // CRC_16_CCITT_KERMIT
		16u, // CRC_16_IBM
		16u, // CRC_16_XMODEM
		32u, // CRC_32
		32u, // CRC_32_BZIP2
		32u, // CRC_32_CASTAGNOLI
		32u, // CRC_32_MPEG2
		32u, // CRC_32_POSIX
		64u, // CRC_64_XZ
	};

	static const uint64_t divisorTable[] = {
		static_cast<uint64_t>(0x0000000000001021ull), // CRC_16_AUG_CCITT
		static_cast<uint64_t>(0x0000000000001021ull), // CRC_16_CCITT_KERMIT
		static_cast<uint64_t>(0x0000000000008005ull), // CRC_16_IBM
		static_cast<uint64_t>(0x0000000000001021ull), // CRC_16_XMODEM
		static_cast<uint64_t>(0x0000000004C11DB7ull), // CRC_32
		static_cast<uint64_t>(0x0000000004C11DB7ull), // CRC_32_BZIP2
		static_cast<uint64_t>(0x000000001EDC6F41ull), // CRC_32_CASTAGNOLI
		static_cast<uint64_t>(0x0000000004C11DB7ull), // CRC_32_MPEG2
		static_cast<uint64_t>(0x0000000004C11DB7ull), // CRC_32_POSIX
		static_cast<uint64_t>(0x42F0E1EBA9EA3693ull), // CRC_64_XZ
	};

	static const uint64_t initialTable[] = {
		static_cast<uint64_t>(0x000000000000FFFFull), // CRC_16_AUG_CCITT
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_16_CCITT_KERMIT
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_16_IBM
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_16_XMODEM
		static_cast<uint64_t>(0x00000000FFFFFFFFull), // CRC_32
		static_cast<uint64_t>(0x00000000FFFFFFFFull), // CRC_32_BZIP2
		static_cast<uint64_t>(0x00000000FFFFFFFFull), // CRC_32_CASTAGNOLI
		static_cast<uint64_t>(0x00000000FFFFFFFFull), // CRC_32_MPEG2
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_32_POSIX
		static_cast<uint64_t>(0xFFFFFFFFFFFFFFFFull), // CRC_64_XZ
	};

	static const uint64_t finalTable[] = {
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_16_AUG_CCITT
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_16_CCITT_KERMIT
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_16_IBM
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_16_XMODEM
		static_cast<uint64_t>(0x00000000FFFFFFFFull), // CRC_32
		static_cast<uint64_t>(0x00000000FFFFFFFFull), // CRC_32_BZIP2
		static_cast<uint64_t>(0x00000000FFFFFFFFull), // CRC_32_CASTAGNOLI
		static_cast<uint64_t>(0x0000000000000000ull), // CRC_32_MPEG2
		static_cast<uint64_t>(0x00000000FFFFFFFFull), // CRC_32_POSIX
		static_cast<uint64_t>(0xFFFFFFFFFFFFFFFFull), // CRC_64_XZ
	};

	static const int flagsTable[] = {
		0,															// CRC_16_AUG_CCITT
		CRCDigest::REFLECT_INPUT_BYTES | CRCDigest::REFLECT_DIGEST,	// CRC_16_CCITT_KERMIT
		CRCDigest::REFLECT_INPUT_BYTES | CRCDigest::REFLECT_DIGEST,	// CRC_16_IBM
		0,															// CRC_16_XMODEM
		CRCDigest::REFLECT_INPUT_BYTES | CRCDigest::REFLECT_DIGEST,	// CRC_32
		0,															// CRC_32_BZIP2
		CRCDigest::REFLECT_INPUT_BYTES | CRCDigest::REFLECT_DIGEST,	// CRC_32_CASTAGNOLI
		0,															// CRC_32_MPEG2
		0,															// CRC_32_POSIX
		CRCDigest::REFLECT_INPUT_BYTES | CRCDigest::REFLECT_DIGEST,	// CRC_64_XZ
	};

	CRCDigest::CRCDigest(Polynomial polynomial) : order(static_cast<MemorySize>(orderTable[polynomial])),
			divisor(divisorTable[polynomial]), finalXOR(finalTable[polynomial]), flags(flagsTable[polynomial]) {
		setInitialValue(initialTable[polynomial]);
	}

	CRCDigest::CRCDigest(const CRCDigest& digest) : DigestAlgorithm(digest), order(digest.order),
			divisor(digest.divisor), input(digest.input), initialValue(digest.input),
			finalXOR(digest.finalXOR), flags(digest.flags) {}

	void CRCDigest::setInitialValue(uint64_t newInitialValue) {
		input = initialValue = newInitialValue;
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < order; ++u) {
			unsigned lo = static_cast<unsigned>(input) & 1u;
			if(lo)
				input ^= divisor;
			input >>= 1;
			if(lo)
				input |= static_cast<uint64_t>(1u) << (order - static_cast<MemorySize>(1u));
		}
	}

	MemorySize CRCDigest::getKeySize() const {
		return Algorithm::bytesNeeded(order);
	}

	void CRCDigest::reset() {
		input = initialValue;
	}

	void CRCDigest::digestMessage(const char* buffer, MemorySize count) {
		while(count--) {
			unsigned u, byte = static_cast<unsigned>(static_cast<unsigned char>(*buffer++));
			if(flags & REFLECT_INPUT_BYTES) {
				unsigned reflected = 0u;
				for(u = 0u; u < 8u; ++u) {
					unsigned bit = (byte >> u) & 1u;
					reflected |= bit << (7u - u);
				}
				byte = reflected;
			}
			for(u = 0u; u < 8u; ++u) {
				unsigned hi = static_cast<unsigned>(input >> (order - static_cast<MemorySize>(1u))) & 1u;
				input = (input << 1) | static_cast<uint64_t>((byte >> (7u - u)) & 1u);
				if(hi)
					input ^= divisor;
			}
		}
	}

	uint64_t CRCDigest::getRemainder() {
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < order; ++u) {
			unsigned hi = static_cast<unsigned>(input >> (order - static_cast<MemorySize>(1u))) & 1u;
			input <<= 1;
			if(hi)
				input ^= divisor;
		}
		if(flags & REFLECT_DIGEST) {
			uint64_t reflected = static_cast<uint64_t>(0u);
			for(u = static_cast<MemorySize>(0u); u < order; ++u) {
				unsigned bit = (input >> u) & static_cast<uint64_t>(1u);
				reflected |= bit << (order - u - static_cast<MemorySize>(1u));
			}
			input = reflected;
		}
		input ^= finalXOR;
		return input & (~static_cast<uint64_t>(0u) >> (static_cast<MemorySize>(64u) - order));
	}

	void CRCDigest::getDigest(char* buffer) {
		uint64_t digested = Endianness<uint64_t>::convertBig(getRemainder());
		MemorySize bytes = Algorithm::bytesNeeded(order);
		memcpy(buffer, reinterpret_cast<const char*>(&digested) + (static_cast<MemorySize>(8u) - bytes),
				static_cast<size_t>(bytes));
	}

}}
