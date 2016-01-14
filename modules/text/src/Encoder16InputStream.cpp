#include <cstring>
#include <redstrain/error/handler.hpp>

#include "Encoder16.hpp"
#include "RenditionIOError.hpp"
#include "Encoder16InputStream.hpp"

using redengine::io::InputStream;
using redengine::util::MemorySize;
using redengine::error::wrapError;

namespace redengine {
namespace text {

	Encoder16InputStream::Encoder16InputStream(InputStream<Char16>& input, Encoder16& encoder)
			: input(input), encoder(encoder), bufferFill(static_cast<MemorySize>(0u)),
			bufferOffset(static_cast<MemorySize>(0u)) {}

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

	MemorySize Encoder16InputStream::readBlock(char* buffer, MemorySize bufferSize) {
		MemorySize outcount, consumed;
		do {
			if(bufferOffset >= bufferFill) {
				MemorySize count = input.read(charBuffer,
						static_cast<MemorySize>(REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE));
				if(!count) {
					atEnd = true;
					try {
						encoder.endEncoding();
					}
					catch(const RenditionError& error) {
						wrapError<RenditionError, RenditionIOError>(error);
					}
					return static_cast<MemorySize>(0u);
				}
				bufferFill = count;
				bufferOffset = static_cast<MemorySize>(0u);
			}
			consumed = encoder.encodeBlock(charBuffer + bufferOffset, bufferFill - bufferOffset,
					buffer, bufferSize, outcount);
			bufferOffset += consumed;
		} while(!outcount);
		return outcount;
	}

}}
