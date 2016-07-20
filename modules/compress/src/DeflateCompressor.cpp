#include <redstrain/util/DeleteArray.hpp>

#include "DeflateCompressor.hpp"
#include "IllegalDeflateHashExponentError.hpp"
#include "IllegalDeflateWindowExponentError.hpp"

using redengine::util::MemorySize;
using redengine::util::DeleteArray;

namespace redengine {
namespace compress {

	DeflateCompressor::DeflateCompressor() : windowExponent(DeflateCompressor::DEFAULT_WINDOW_EXPONENT),
			hashExponent(DeflateCompressor::DEFAULT_HASH_EXPONENT), slidingWindow(NULL), olderStrings(NULL),
			hashChains(NULL) {}

	DeflateCompressor::DeflateCompressor(const DeflateCompressor& compressor)
			: Compressor(compressor), windowExponent(compressor.windowExponent),
			hashExponent(compressor.hashExponent), slidingWindow(NULL), olderStrings(NULL), hashChains(NULL) {}

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
		//TODO: throw if bad state
		windowExponent = exponent;
		if(slidingWindow) {
			delete[] slidingWindow;
			slidingWindow = NULL;
			delete[] olderStrings;
			olderStrings = NULL;
		}
	}

	void DeflateCompressor::setHashExponend(MemorySize exponent) {
		if(exponent < static_cast<MemorySize>(8u) || exponent > static_cast<MemorySize>(16u))
			throw IllegalDeflateHashExponentError(exponent);
		//TODO: throw if bad state
		hashExponent = exponent;
		if(hashChains) {
			delete[] hashChains;
			hashChains = NULL;
		}
	}

	/*
	MemorySize DeflateCompressor::compressBlock(const char* inBuffer, MemorySize inSize,
			char* outBuffer, MemorySize outSize, MemorySize& outCount) {
		MemorySize windowSize = static_cast<MemorySize>(1u) << windowExponent;
		MemorySize hashSize = static_cast<MemorySize>(1u) << hashExponent;
		if(!slidingWindow) {
			DeleteArray<char> sw(new char[windowSize]);
			olderStrings = new WindowOffset[windowSize];
			slidingWindow = sw.set();
		}
		if(!hashChains)
			hashChains = new WindowOffset[hashSize];
		//TODO
	}
	*/

}}
