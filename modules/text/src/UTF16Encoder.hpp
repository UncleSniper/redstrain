#ifndef REDSTRAIN_MOD_TEXT_UTF16ENCODER_HPP
#define REDSTRAIN_MOD_TEXT_UTF16ENCODER_HPP

#include "TextCodec.hpp"
#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UTF16Encoder : public TextCodec<Char32, Char16> {

	  private:
		unsigned pending;
		Char16 partial;

	  public:
		UTF16Encoder();
		UTF16Encoder(const UTF16Encoder&);

		virtual util::MemorySize transcodeBlock(const Char32*, util::MemorySize,
				Char16*, util::MemorySize, util::MemorySize&);
		virtual void endCodeUnit();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF16ENCODER_HPP */
