#ifndef REDSTRAIN_MOD_TEXT_TEXTCODECINPUTSTREAM_HPP
#define REDSTRAIN_MOD_TEXT_TEXTCODECINPUTSTREAM_HPP

#include <cstring>
#include <redstrain/io/InputStream.hpp>
#include <redstrain/error/handler.hpp>

#include "TextCodec.hpp"
#include "RenditionIOError.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	template<typename SourceT, typename TargetT>
	class TextCodecInputStream : public io::InputStream<TargetT> {

	  private:
		io::InputStream<SourceT>& input;
		TextCodec<SourceT, TargetT>& codec;
		SourceT sourceBuffer[REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE];
		util::MemorySize bufferFill, bufferOffset;

	  protected:
		TextCodecInputStream(const TextCodecInputStream& stream) : io::Stream(stream),
				io::InputStream<TargetT>(stream), input(stream.input), codec(stream.codec),
				bufferFill(stream.bufferFill), bufferOffset(stream.bufferOffset) {
			if(bufferOffset < bufferFill)
				memcpy(sourceBuffer + bufferOffset, stream.sourceBuffer + bufferOffset,
						static_cast<size_t>(bufferFill - bufferOffset));
		}

		virtual util::MemorySize readBlock(TargetT* buffer, util::MemorySize bufferSize) {
			if(!bufferSize)
				return static_cast<util::MemorySize>(0u);
			util::MemorySize outcount, consumed;
			do {
				if(bufferOffset >= bufferFill) {
					util::MemorySize count = input.read(sourceBuffer,
							static_cast<util::MemorySize>(REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE));
					if(!count) {
						try {
							codec.transcodeBlock(NULL, static_cast<util::MemorySize>(0u),
									buffer, bufferSize, outcount);
							if(outcount)
								return outcount;
							this->atEnd = true;
							codec.endCodeUnit();
						}
						catch(const RenditionError& error) {
							error::wrapError<RenditionError, RenditionIOError>(error);
						}
						return static_cast<util::MemorySize>(0u);
					}
					bufferFill = count;
					bufferOffset = static_cast<util::MemorySize>(0u);
				}
				try {
					consumed = codec.transcodeBlock(sourceBuffer + bufferOffset, bufferFill - bufferOffset,
							buffer, bufferSize, outcount);
				}
				catch(const RenditionError& error) {
					error::wrapError<RenditionError, RenditionIOError>(error);
				}
				bufferOffset += consumed;
			} while(!outcount);
			return outcount;
		}

	  public:
		TextCodecInputStream(io::InputStream<SourceT>& input, TextCodec<SourceT, TargetT>& codec)
				: input(input), codec(codec), bufferFill(static_cast<util::MemorySize>(0u)),
				bufferOffset(static_cast<util::MemorySize>(0u)) {}

		inline io::InputStream<SourceT>& getBackingInputStream() {
			return input;
		}

		inline const io::InputStream<SourceT>& getBackingInputStream() const {
			return input;
		}

		inline TextCodec<SourceT, TargetT>& getTextCodec() {
			return codec;
		}

		inline const TextCodec<SourceT, TargetT>& getTextCodec() const {
			return codec;
		}

		virtual void close() {
			input.close();
		}

	};

	typedef TextCodecInputStream<char, Char16> Decoder16InputStream;
	typedef TextCodecInputStream<Char16, char> Encoder16InputStream;
	typedef TextCodecInputStream<char, Char32> Decoder32InputStream;
	typedef TextCodecInputStream<Char32, char> Encoder32InputStream;

}}

#endif /* REDSTRAIN_MOD_TEXT_TEXTCODECINPUTSTREAM_HPP */
