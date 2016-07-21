/* Sooooo, this implementation draws parallels to zlib,
 * but doesn't actually use any zlib code. The way I see
 * it, this should make it okay to blatantly rip off part
 * of their approach in this here public domain thingy.
 * The one thing that's *actually* "stolen" is the table
 * of configurations -- this, of course, is mostly to
 * keep the effects of the compression levels consistent
 * with everyone's favorite implementation, so that the
 * meaning of those levels will be what users might
 * expect.
 */

#include <redstrain/util/DeleteArray.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "DeflateCompressor.hpp"
#include "IllegalDeflateFunctionError.hpp"
#include "CompressionInputAfterEndError.hpp"
#include "IllegalDeflateHashExponentError.hpp"
#include "IllegalDeflateWindowExponentError.hpp"
#include "IllegalDeflateCompressionLevelError.hpp"
#include "CannotModifyCompressionParameterError.hpp"

using redengine::util::MemorySize;
using redengine::util::DeleteArray;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;

namespace redengine {
namespace compress {

	// ======== Configuration ========

	DeflateCompressor::Configuration::Configuration(unsigned good, unsigned lazy, unsigned nice, unsigned chain,
			DeflateFunction function) : good(good), lazy(lazy), nice(nice), chain(chain), function(function) {
		switch(function) {
			case DFL_F_STORE:
			case DFL_F_FAST:
			case DFL_F_SLOW:
				break;
			default:
				throw IllegalDeflateFunctionError(static_cast<int>(function));
		}
	}

	DeflateCompressor::Configuration::Configuration(const Configuration& configuration)
			: good(configuration.good), lazy(configuration.lazy), nice(configuration.nice),
			chain(configuration.chain), function(configuration.function) {}

	// ======== DeflateCompressor ========

	const DeflateCompressor::Configuration
			DeflateCompressor::CONFIGURATIONS[DeflateCompressor::CONFIGURATION_COUNT] = {
		/*0*/ DeflateCompressor::Configuration( 0u,   0u,   0u,    0u, DFL_F_STORE),
		/*1*/ DeflateCompressor::Configuration( 4u,   4u,   8u,    4u, DFL_F_FAST),
		/*2*/ DeflateCompressor::Configuration( 4u,   5u,  16u,    8u, DFL_F_FAST),
		/*3*/ DeflateCompressor::Configuration( 4u,   6u,  32u,   32u, DFL_F_FAST),
		/*4*/ DeflateCompressor::Configuration( 4u,   4u,  16u,   16u, DFL_F_SLOW),
		/*5*/ DeflateCompressor::Configuration( 8u,  16u,  32u,   32u, DFL_F_SLOW),
		/*6*/ DeflateCompressor::Configuration( 8u,  16u, 128u,  128u, DFL_F_SLOW),
		/*7*/ DeflateCompressor::Configuration( 8u,  32u, 128u,  256u, DFL_F_SLOW),
		/*8*/ DeflateCompressor::Configuration(32u, 128u, 258u, 1024u, DFL_F_SLOW),
		/*9*/ DeflateCompressor::Configuration(32u, 258u, 258u, 4096u, DFL_F_SLOW),
	};

	static const DeflateCompressor::Configuration& getConfigurationByLevel(unsigned compressionLevel) {
		if(compressionLevel >= DeflateCompressor::CONFIGURATION_COUNT)
			throw IllegalDeflateCompressionLevelError(compressionLevel);
		return DeflateCompressor::CONFIGURATIONS[compressionLevel];
	}

	DeflateCompressor::DeflateCompressor() : windowExponent(DeflateCompressor::DEFAULT_WINDOW_EXPONENT),
			hashExponent(DeflateCompressor::DEFAULT_HASH_EXPONENT),
			configuration(DeflateCompressor::CONFIGURATIONS[DeflateCompressor::DEFAULT_CONFIGURATION]),
			state(ST_INIT), slidingWindow(NULL), olderStrings(NULL), hashChains(NULL) {}

	DeflateCompressor::DeflateCompressor(unsigned compressionLevel)
			: windowExponent(DeflateCompressor::DEFAULT_WINDOW_EXPONENT),
			hashExponent(DeflateCompressor::DEFAULT_HASH_EXPONENT),
			configuration(getConfigurationByLevel(compressionLevel)), state(ST_INIT), slidingWindow(NULL),
			olderStrings(NULL), hashChains(NULL) {}

	DeflateCompressor::DeflateCompressor(const Configuration& configuration)
			: windowExponent(DeflateCompressor::DEFAULT_WINDOW_EXPONENT),
			hashExponent(DeflateCompressor::DEFAULT_HASH_EXPONENT), configuration(configuration),
			state(ST_INIT), slidingWindow(NULL), olderStrings(NULL), hashChains(NULL) {}

	DeflateCompressor::DeflateCompressor(const DeflateCompressor& compressor)
			: Compressor(compressor), windowExponent(compressor.windowExponent),
			hashExponent(compressor.hashExponent), configuration(compressor.configuration),
			state(compressor.state), slidingWindow(NULL), olderStrings(NULL), hashChains(NULL) {}

	DeflateCompressor::~DeflateCompressor() {
		if(slidingWindow) {
			delete[] slidingWindow;
			delete[] olderStrings;
		}
		if(hashChains)
			delete[] hashChains;
	}

	void DeflateCompressor::setWindowExponent(MemorySize exponent) {
		if(exponent < static_cast<MemorySize>(8u) || exponent > static_cast<MemorySize>(15u))
			throw IllegalDeflateWindowExponentError(exponent);
		if(state != ST_INIT)
			throw CannotModifyCompressionParameterError();
		windowExponent = exponent;
		if(slidingWindow) {
			delete[] slidingWindow;
			slidingWindow = NULL;
			delete[] olderStrings;
			olderStrings = NULL;
		}
	}

	void DeflateCompressor::setHashExponent(MemorySize exponent) {
		if(exponent < static_cast<MemorySize>(8u) || exponent > static_cast<MemorySize>(16u))
			throw IllegalDeflateHashExponentError(exponent);
		if(state != ST_INIT)
			throw CannotModifyCompressionParameterError();
		hashExponent = exponent;
		if(hashChains) {
			delete[] hashChains;
			hashChains = NULL;
		}
	}

	MemorySize DeflateCompressor::writeHeader(char*, MemorySize) {
		return static_cast<MemorySize>(0u);
	}

	MemorySize DeflateCompressor::writeTrailer(char*, MemorySize) {
		return static_cast<MemorySize>(0u);
	}

	void DeflateCompressor::ensureBuffers() {
		if(!slidingWindow) {
			MemorySize windowSize = static_cast<MemorySize>(1u) << windowExponent;
			DeleteArray<char> sw(new char[windowSize]);
			olderStrings = new WindowOffset[windowSize];
			slidingWindow = sw.set();
		}
		if(!hashChains) {
			MemorySize hashSize = static_cast<MemorySize>(1u) << hashExponent;
			hashChains = new WindowOffset[hashSize];
		}
	}

	MemorySize DeflateCompressor::compressBlock(const char* inBuffer, MemorySize inSize,
			char* outBuffer, MemorySize outSize, MemorySize& outCount) {
		return advanceCompression(inBuffer, inSize, outBuffer, outSize, outCount, false);
	}

	MemorySize DeflateCompressor::endCompression(char* outBuffer, MemorySize outSize) {
		MemorySize outCount;
		advanceCompression(NULL, static_cast<MemorySize>(0u), outBuffer, outSize, outCount, true);
		return outCount;
	}

	MemorySize DeflateCompressor::advanceCompression(const char* inBuffer, MemorySize inSize,
			char* outBuffer, MemorySize outSize, MemorySize& outCount, bool lastBlock) {
		switch(state) {
			case ST_INIT:
				state = ST_HEADER;
			case ST_HEADER:
				if(!outSize) {
					outCount = static_cast<MemorySize>(0u);
					return static_cast<MemorySize>(0u);
				}
				outCount = writeHeader(outBuffer, outSize);
				if(outCount)
					return static_cast<MemorySize>(0u);
				//TODO: set state
				break;
			case ST_TRAILER:
				if(inSize)
					throw CompressionInputAfterEndError();
				if(!outSize) {
					outCount = static_cast<MemorySize>(0u);
					return static_cast<MemorySize>(0u);
				}
				outCount = writeTrailer(outBuffer, outSize);
				if(outCount)
					return static_cast<MemorySize>(0u);
				state = ST_DONE;
				break;
			case ST_DONE:
				if(inSize)
					throw CompressionInputAfterEndError();
				outCount = static_cast<MemorySize>(0u);
				return static_cast<MemorySize>(0u);
			default:
				break;
		}
		ensureBuffers();
		MemorySize consumed;
		switch(configuration.function) {
			case DFL_F_STORE:
				consumed = deflateStore(inBuffer, inSize, outBuffer, outSize, outCount, lastBlock);
				break;
			case DFL_F_FAST:
				consumed = deflateFast(inBuffer, inSize, outBuffer, outSize, outCount, lastBlock);
				break;
			case DFL_F_SLOW:
				consumed = deflateSlow(inBuffer, inSize, outBuffer, outSize, outCount, lastBlock);
				break;
			default:
				throw ProgrammingError("Unrecognized deflate function ID: "
						+ StringUtils::toString(static_cast<int>(configuration.function)));
		}
		if(lastBlock && consumed == inSize && !outCount) {
			state = ST_TRAILER;
			if(outSize) {
				outCount = writeTrailer(outBuffer, outSize);
				if(!outCount)
					state = ST_DONE;
			}
		}
		return consumed;
	}

	/*
	MemorySize DeflateCompressor::deflateStore(const char* inBuffer, MemorySize inSize,
			char* outBuffer, MemorySize outSize, MemorySize& outCount) {
		//TODO
	}

	MemorySize DeflateCompressor::deflateFast(const char* inBuffer, MemorySize inSize,
			char* outBuffer, MemorySize outSize, MemorySize& outCount) {
		//TODO
	}

	MemorySize DeflateCompressor::deflateSlow(const char* inBuffer, MemorySize inSize,
			char* outBuffer, MemorySize outSize, MemorySize& outCount) {
		//TODO
	}
	*/

}}
