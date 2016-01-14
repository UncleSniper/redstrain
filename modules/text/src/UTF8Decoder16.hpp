#ifndef REDSTRAIN_MOD_TEXT_UTF8DECODER16_HPP
#define REDSTRAIN_MOD_TEXT_UTF8DECODER16_HPP

#include "Decoder16.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UTF8Decoder16 : public Decoder16 {

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

		virtual util::MemorySize decodeBlock(const char*, util::MemorySize,
				Char16*, util::MemorySize, util::MemorySize&);
		virtual void endDecoding();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF8DECODER16_HPP */
