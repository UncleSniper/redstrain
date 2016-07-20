#ifndef REDSTRAIN_MOD_COMPRESS_DEFLATECOMPRESSOR_HPP
#define REDSTRAIN_MOD_COMPRESS_DEFLATECOMPRESSOR_HPP

#include <stdint.h>

#include "Compressor.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API DeflateCompressor : public Compressor {

	  private:
		typedef uint16_t WindowOffset;

	  public:
		static const util::MemorySize DEFAULT_WINDOW_EXPONENT = static_cast<util::MemorySize>(15u);
		static const util::MemorySize DEFAULT_HASH_EXPONENT = static_cast<util::MemorySize>(15u);

	  private:
		util::MemorySize windowExponent, hashExponent;
		char* slidingWindow;
		WindowOffset *olderStrings, *hashChains;

	  public:
		DeflateCompressor();
		DeflateCompressor(const DeflateCompressor&);
		virtual ~DeflateCompressor();

		inline util::MemorySize getWindowExponent() const {
			return windowExponent;
		}

		void setWindowExponent(util::MemorySize);

		inline util::MemorySize getHashExponent() const {
			return hashExponent;
		}

		void setHashExponend(util::MemorySize);

		virtual util::MemorySize compressBlock(const char*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&) = 0;
		virtual void endCompression();

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_DEFLATECOMPRESSOR_HPP */
