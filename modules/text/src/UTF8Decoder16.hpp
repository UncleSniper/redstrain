#ifndef REDSTRAIN_MOD_TEXT_UTF8DECODER16_HPP
#define REDSTRAIN_MOD_TEXT_UTF8DECODER16_HPP

#include "TextCodec.hpp"
#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UTF8Decoder16 : public TextCodec<char, Char16> {

	  private:
		enum State {
			ST_NONE,
			ST_SEQ2BYTE0,
			ST_SEQ3BYTE0,
			ST_SEQ3BYTE1,
			ST_ERROR
		};

	  private:
		State state;
		Char16 partial;

	  public:
		UTF8Decoder16();
		UTF8Decoder16(const UTF8Decoder16&);

		virtual util::MemorySize transcodeBlock(const char*, util::MemorySize,
				Char16*, util::MemorySize, util::MemorySize&);
		virtual void endCodeUnit();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF8DECODER16_HPP */
