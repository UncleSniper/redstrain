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

		void reset();

		virtual Char16 getInverseBreakChar(Char32) const;
		virtual util::MemorySize transcodeBlock(const Char16*, util::MemorySize,
				Char32*, util::MemorySize, util::MemorySize&);
		virtual void endCodeUnit();

		static util::MemorySize decodeSingleChar(const Char16*, util::MemorySize, Char32&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF16DECODER_HPP */
