#ifndef REDSTRAIN_MOD_COMPRESS_COMPRESSOR_HPP
#define REDSTRAIN_MOD_COMPRESS_COMPRESSOR_HPP

#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API Compressor {

	  public:
		Compressor();
		Compressor(const Compressor&);
		virtual ~Compressor();

		virtual util::MemorySize compressBlock(const char*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&) = 0;
		virtual util::MemorySize endCompression(char*, util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_COMPRESSOR_HPP */
