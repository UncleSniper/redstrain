#ifndef REDSTRAIN_MOD_COMPRESS_DEFLATECOMPRESSOR_HPP
#define REDSTRAIN_MOD_COMPRESS_DEFLATECOMPRESSOR_HPP

#include <stdint.h>

#include "Compressor.hpp"
#include "deflate.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API DeflateCompressor : public Compressor {

	  public:
		class REDSTRAIN_COMPRESS_API Configuration {

		  public:
			const unsigned good, lazy, nice, chain;
			const DeflateFunction function;

		  public:
			Configuration(unsigned, unsigned, unsigned, unsigned, DeflateFunction);
			Configuration(const Configuration&);

		};

	  private:
		enum State {
			ST_INIT,
			ST_HEADER,
			ST_TRAILER,
			ST_DONE
		};

	  private:
		typedef uint16_t WindowOffset;

	  public:
		static const util::MemorySize DEFAULT_WINDOW_EXPONENT = static_cast<util::MemorySize>(15u);
		static const util::MemorySize DEFAULT_HASH_EXPONENT = static_cast<util::MemorySize>(15u);

		static const unsigned CONFIGURATION_COUNT = 10u;
		static const unsigned DEFAULT_CONFIGURATION = 3u;
		static const Configuration CONFIGURATIONS[CONFIGURATION_COUNT];

	  private:
		util::MemorySize windowExponent, hashExponent;
		const Configuration configuration;
		State state;
		char* slidingWindow;
		WindowOffset *olderStrings, *hashChains;

	  private:
		void ensureBuffers();
		util::MemorySize advanceCompression(const char*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&, bool);
		util::MemorySize deflateStore(const char*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&, bool);
		util::MemorySize deflateFast(const char*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&, bool);
		util::MemorySize deflateSlow(const char*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&, bool);

	  protected:
		virtual util::MemorySize writeHeader(char*, util::MemorySize);
		virtual util::MemorySize writeTrailer(char*, util::MemorySize);

	  public:
		DeflateCompressor();
		DeflateCompressor(unsigned);
		DeflateCompressor(const Configuration&);
		DeflateCompressor(const DeflateCompressor&);
		virtual ~DeflateCompressor();

		inline util::MemorySize getWindowExponent() const {
			return windowExponent;
		}

		void setWindowExponent(util::MemorySize);

		inline util::MemorySize getHashExponent() const {
			return hashExponent;
		}

		void setHashExponent(util::MemorySize);

		inline const Configuration& getConfiguration() const {
			return configuration;
		}

		virtual util::MemorySize compressBlock(const char*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&);
		virtual util::MemorySize endCompression(char*, util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_DEFLATECOMPRESSOR_HPP */
