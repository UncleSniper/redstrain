#ifndef REDSTRAIN_MOD_TEXT_DECODER16_HPP
#define REDSTRAIN_MOD_TEXT_DECODER16_HPP

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API Decoder16 {

	  public:
		Decoder16();
		Decoder16(const Decoder16&);
		virtual ~Decoder16();

		virtual size_t decodeBlock(const char*, size_t, Char16*, size_t, size_t&) = 0;
		virtual void endDecoding();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DECODER16_HPP */
