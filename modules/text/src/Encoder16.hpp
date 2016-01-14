#ifndef REDSTRAIN_MOD_TEXT_ENCODER16_HPP
#define REDSTRAIN_MOD_TEXT_ENCODER16_HPP

#include <redstrain/util/types.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API Encoder16 {

	  public:
		Encoder16();
		Encoder16(const Encoder16&);
		virtual ~Encoder16();

		virtual util::MemorySize encodeBlock(const Char16*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&) = 0;
		virtual void endEncoding();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_ENCODER16_HPP */
