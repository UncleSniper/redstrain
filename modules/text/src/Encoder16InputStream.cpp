#include <cstring>
#include <redstrain/error/handler.hpp>

#include "Encoder16.hpp"
#include "RenditionIOError.hpp"
#include "Encoder16InputStream.hpp"

using redengine::io::InputStream;
using redengine::error::wrapError;

namespace redengine {
namespace text {

	Encoder16InputStream::Encoder16InputStream(InputStream<Char16>& input, Encoder16& encoder)
			: input(input), encoder(encoder), bufferFill(static_cast<size_t>(0u)),
			bufferOffset(static_cast<size_t>(0u)) {}

	Encoder16InputStream::Encoder16InputStream(const Encoder16InputStream& stream)
			: Stream(stream), InputStream<char>(stream), input(stream.input), encoder(stream.encoder),
			bufferFill(stream.bufferFill), bufferOffset(stream.bufferOffset) {
		if(bufferOffset < bufferFill)
			memcpy(charBuffer + bufferOffset, stream.charBuffer + bufferOffset,
					(bufferFill - bufferOffset) * sizeof(Char16));
	}

	void Encoder16InputStream::close() {
		input.close();
	}

	size_t Encoder16InputStream::readBlock(char* buffer, size_t bufferSize) {
		size_t outcount, consumed;
		do {
			if(bufferOffset >= bufferFill) {
				size_t count = input.read(charBuffer, static_cast<size_t>(REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE));
				if(!count) {
					atEnd = true;
					try {
						encoder.endEncoding();
					}
					catch(const RenditionError& error) {
						wrapError<RenditionError, RenditionIOError>(error);
					}
					return static_cast<size_t>(0u);
				}
				bufferFill = count;
				bufferOffset = static_cast<size_t>(0u);
			}
			consumed = encoder.encodeBlock(charBuffer + bufferOffset, bufferFill - bufferOffset,
					buffer, bufferSize, outcount);
			bufferOffset += consumed;
		} while(!outcount);
		return outcount;
	}

}}
