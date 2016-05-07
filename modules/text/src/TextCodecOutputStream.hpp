#ifndef REDSTRAIN_MOD_TEXT_TEXTCODECOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_TEXT_TEXTCODECOUTPUTSTREAM_HPP

#include <redstrain/io/OutputStream.hpp>
#include <redstrain/error/handler.hpp>

#include "TextCodec.hpp"
#include "RenditionIOError.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	template<typename SourceT, typename TargetT>
	class TextCodecOutputStream : public io::OutputStream<SourceT> {

	  private:
		io::OutputStream<TargetT>& output;
		TextCodec<SourceT, TargetT>& codec;

	  protected:
		TextCodecOutputStream(const TextCodecOutputStream& stream)
				: io::Stream(stream), io::OutputStream<SourceT>(stream), output(stream.output),
				codec(stream.codec) {}

		virtual void writeBlock(const SourceT* buffer, util::MemorySize bufferSize) {
			TargetT targetBuffer[REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE];
			util::MemorySize outcount, consumed;
			while(bufferSize) {
				try {
					consumed = codec.transcodeBlock(buffer, bufferSize, targetBuffer,
							static_cast<util::MemorySize>(REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE), outcount);
				}
				catch(const RenditionError& error) {
					error::wrapError<RenditionError, RenditionIOError>(error);
				}
				output.write(targetBuffer, outcount);
				buffer += consumed;
				bufferSize -= consumed;
			}
		}

	  public:
		TextCodecOutputStream(io::OutputStream<TargetT>& output, TextCodec<SourceT, TargetT>& codec)
				: output(output), codec(codec) {}

		inline io::OutputStream<TargetT>& getBackingOutputStream() {
			return output;
		}

		inline const io::OutputStream<TargetT>& getBackingOutputStream() const {
			return output;
		}

		inline TextCodec<SourceT, TargetT>& getTextCodec() {
			return codec;
		}

		inline const TextCodec<SourceT, TargetT>& getTextCodec() const {
			return codec;
		}

		void flushCodeUnit() {
			TargetT targetBuffer[REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE];
			util::MemorySize outcount;
			try {
				for(;;) {
					codec.transcodeBlock(NULL, static_cast<util::MemorySize>(0u), targetBuffer,
							static_cast<util::MemorySize>(REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE), outcount);
					if(!outcount)
						break;
					output.write(targetBuffer, outcount);
				}
				codec.endCodeUnit();
			}
			catch(const RenditionError& error) {
				error::wrapError<RenditionError, RenditionIOError>(error);
			}
		}

		virtual void close() {
			flushCodeUnit();
			output.close();
		}

	};

	typedef TextCodecOutputStream<char, Char16> Decoder16OutputStream;
	typedef TextCodecOutputStream<Char16, char> Encoder16OutputStream;
	typedef TextCodecOutputStream<char, Char32> Decoder32OutputStream;
	typedef TextCodecOutputStream<Char32, char> Encoder32OutputStream;

}}

#endif /* REDSTRAIN_MOD_TEXT_TEXTCODECOUTPUTSTREAM_HPP */
