#include <cstring>
#include <redstrain/error/handler.hpp>

#include "Decoder16.hpp"
#include "RenditionIOError.hpp"
#include "Decoder16InputStream.hpp"

using redengine::io::InputStream;
using redengine::error::wrapError;

namespace redengine {
namespace text {

	Decoder16InputStream::Decoder16InputStream(InputStream<char>& input, Decoder16& decoder)
			: input(input), decoder(decoder), bufferFill(static_cast<size_t>(0u)),
			bufferOffset(static_cast<size_t>(0u)) {}

	Decoder16InputStream::Decoder16InputStream(const Decoder16InputStream& stream)
			: Stream(stream), InputStream<Char16>(stream), input(stream.input), decoder(stream.decoder),
			bufferFill(stream.bufferFill), bufferOffset(stream.bufferOffset) {
		if(bufferOffset < bufferFill)
			memcpy(byteBuffer + bufferOffset, stream.byteBuffer + bufferOffset, bufferFill - bufferOffset);
	}

	void Decoder16InputStream::close() {
		input.close();
	}

	size_t Decoder16InputStream::readBlock(Char16* buffer, size_t bufferSize) {
		size_t outcount, consumed;
		do {
			if(bufferOffset >= bufferFill) {
				size_t count = input.read(byteBuffer, static_cast<size_t>(REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE));
				if(!count) {
					atEnd = true;
					try {
						decoder.endDecoding();
					}
					catch(const RenditionError& error) {
						wrapError<RenditionError, RenditionIOError>(error);
					}
					return static_cast<size_t>(0u);
				}
				bufferFill = count;
				bufferOffset = static_cast<size_t>(0u);
			}
			consumed = decoder.decodeBlock(byteBuffer + bufferOffset, bufferFill - bufferOffset,
					buffer, bufferSize, outcount);
			bufferOffset += consumed;
		} while(!outcount);
		return outcount;
	}

}}
