#ifndef REDSTRAIN_MOD_TEXT_UTF16DECODER_HPP
#define REDSTRAIN_MOD_TEXT_UTF16DECODER_HPP

#include "TextCodec.hpp"
#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UTF16Decoder16 : public TextCodec<Char16, Char32> {

	  private:
		enum State {
			ST_NONE,
			ST_SPUNIT0,
			ST_ERROR
		};

	  private:
		State state;
		Char16 partial;

	  public:
		UTF16Decoder16();
		UTF16Decoder16(const UTF16Decoder16&);

		virtual util::MemorySize transcodeBlock(const Char16*, util::MemorySize,
				Char32*, util::MemorySize, util::MemorySize&);
		virtual void endCodeUnit();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF16DECODER_HPP */
