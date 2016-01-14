#include <cstring>

#include "ANSIX939Padding.hpp"
#include "IllegalPaddingCodeError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	ANSIX939Padding::ANSIX939Padding() {}

	ANSIX939Padding::ANSIX939Padding(const ANSIX939Padding& padding) : Padding(padding) {}

	void ANSIX939Padding::padBlock(char* data, MemorySize fill, MemorySize blockSize) {
		if(fill >= blockSize)
			return;
		MemorySize rest = blockSize - fill;
		if(rest)
			memset(data + fill, 0, static_cast<size_t>(rest));
		data[blockSize - static_cast<MemorySize>(1u)] = static_cast<char>(static_cast<unsigned char>(rest));
	}

	MemorySize ANSIX939Padding::unpadBlock(char* data, MemorySize blockSize) {
		MemorySize rest = static_cast<MemorySize>(static_cast<unsigned char>(data[blockSize
				- static_cast<MemorySize>(1u)]));
		if(rest > blockSize)
			throw IllegalPaddingCodeError();
		return blockSize - rest;
	}

}}
