#ifndef REDSTRAIN_MOD_TEXT_CHAINEDTEXTCODEC_HPP
#define REDSTRAIN_MOD_TEXT_CHAINEDTEXTCODEC_HPP

#include <cstring>

#include "TextCodec.hpp"
#include "IllegalCodeError.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	template<typename SourceT, typename IntermediateT, typename TargetT>
	class ChainedTextCodec : public TextCodec<SourceT, TargetT> {

	  public:
		static const int FL_MANAGE_FIRST_CODEC  = 001;
		static const int FL_MANAGE_SECOND_CODEC = 002;
		static const int FL_MANAGE_BOTH_CODECS  = 002;
		static const int FL_MASK                = 003;

	  private:
		TextCodec<SourceT, IntermediateT>& firstCodec;
		TextCodec<IntermediateT, TargetT>& secondCodec;
		int flags;
		IntermediateT intBuffer[REDSTRAIN_TEXT_CHAINEDTEXTCODEC_BUFFER_SIZE];
		util::MemorySize bufferOffset, bufferFill;

	  public:
		ChainedTextCodec(TextCodec<SourceT, IntermediateT>& firstCodec,
				TextCodec<IntermediateT, TargetT>& secondCodec, int flags = 0)
				: firstCodec(firstCodec), secondCodec(secondCodec), flags(flags),
				bufferOffset(static_cast<util::MemorySize>(0u)), bufferFill(static_cast<util::MemorySize>(0u)) {}

		ChainedTextCodec(const ChainedTextCodec& codec) : TextCodec<SourceT, TargetT>(codec),
				firstCodec(codec.firstCodec), secondCodec(codec.secondCodec),
				flags(codec.flags & ~ChainedTextCodec::FL_MANAGE_BOTH_CODECS),
				bufferOffset(codec.bufferOffset), bufferFill(codec.bufferFill) {
			if(bufferOffset < bufferFill)
				memcpy(intBuffer + bufferOffset, codec.intBuffer + bufferOffset,
						static_cast<size_t>(static_cast<size_t>(bufferFill - bufferOffset) * sizeof(IntermediateT)));
		}

		virtual ~ChainedTextCodec() {
			if(flags & ChainedTextCodec::FL_MANAGE_FIRST_CODEC)
				delete &firstCodec;
			if(flags & ChainedTextCodec::FL_MANAGE_SECOND_CODEC)
				delete &secondCodec;
		}

		virtual void setBreakChar(TargetT breakChar) {
			IntermediateT ibc = secondCodec.getInverseBreakChar(breakChar);
			this->breakChar = breakChar;
			firstCodec.setBreakChar(ibc);
			secondCodec.setBreakChar(breakChar);
		}

		virtual SourceT getInverseBreakChar(TargetT breakChar) const {
			return firstCodec.getInverseBreakChar(secondCodec.getInverseBreakChar(breakChar));
		}

		virtual util::MemorySize transcodeBlock(const SourceT* input, util::MemorySize insize,
				TargetT* output, util::MemorySize outsize, util::MemorySize& outcount) {
			util::MemorySize c = secondCodec.transcodeBlock(intBuffer + bufferOffset, bufferFill - bufferOffset,
					output, outsize, outcount);
			bufferOffset += c;
			if(outcount || bufferOffset < bufferFill)
				return static_cast<util::MemorySize>(0u);
			util::MemorySize consumed, bf;
			consumed = firstCodec.transcodeBlock(input, insize,
					intBuffer, static_cast<util::MemorySize>(REDSTRAIN_TEXT_CHAINEDTEXTCODEC_BUFFER_SIZE), bf);
			bufferOffset = static_cast<util::MemorySize>(0u);
			bufferFill = bf;
			if(bf)
				bufferOffset += secondCodec.transcodeBlock(intBuffer, bufferFill, output, outsize, outcount);
			return consumed;
		}

		virtual void endCodeUnit() {
			firstCodec.endCodeUnit();
			if(bufferOffset < bufferFill)
				throw IllegalCodeError();
			secondCodec.endCodeUnit();
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CHAINEDTEXTCODEC_HPP */
