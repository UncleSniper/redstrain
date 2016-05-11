#ifndef REDSTRAIN_MOD_TEXT_TEXTCODEC_HPP
#define REDSTRAIN_MOD_TEXT_TEXTCODEC_HPP

#include <redstrain/util/types.hpp>

#include "types.hpp"

namespace redengine {
namespace text {

	template<typename SourceT, typename TargetT>
	class TextCodec {

	  protected:
		TargetT breakChar;

	  public:
		TextCodec() : breakChar(static_cast<TargetT>(0u)) {}
		TextCodec(const TextCodec& codec) : breakChar(codec.breakChar) {}
		virtual ~TextCodec() {}

		inline TargetT getBreakChar() const {
			return breakChar;
		}

		virtual void setBreakChar(TargetT breakChar) {
			this->breakChar = breakChar;
		}

		virtual SourceT getInverseBreakChar(TargetT) const = 0;
		virtual util::MemorySize transcodeBlock(const SourceT*, util::MemorySize,
				TargetT*, util::MemorySize, util::MemorySize&) = 0;
		virtual void endCodeUnit() {}

	};

	typedef TextCodec<Char16, char> Encoder16;
	typedef TextCodec<char, Char16> Decoder16;
	typedef TextCodec<Char32, char> Encoder32;
	typedef TextCodec<char, Char32> Decoder32;
	typedef TextCodec<Char16, Char32> Transcoder1632;
	typedef TextCodec<Char32, Char16> Transcoder3216;

}}

#endif /* REDSTRAIN_MOD_TEXT_TEXTCODEC_HPP */
