#ifndef REDSTRAIN_MOD_TEXT_DECODER16_HPP
#define REDSTRAIN_MOD_TEXT_DECODER16_HPP

#include <redstrain/util/types.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API Decoder16 {

	  public:
		Decoder16();
		Decoder16(const Decoder16&);
		virtual ~Decoder16();

		virtual util::MemorySize decodeBlock(const char*, util::MemorySize,
				Char16*, util::MemorySize, util::MemorySize&) = 0;
		virtual void endDecoding();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DECODER16_HPP */
