#include "Compressor.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace compress {

	Compressor::Compressor() {}

	Compressor::Compressor(const Compressor&) {}

	Compressor::~Compressor() {}

	MemorySize Compressor::endCompression(char*, MemorySize) {
		return static_cast<MemorySize>(0u);
	}

}}
