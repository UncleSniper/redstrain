#include <redstrain/error/handler.hpp>

#include "Encoder16.hpp"
#include "RenditionIOError.hpp"
#include "Encoder16OutputStream.hpp"

using redengine::io::OutputStream;
using redengine::error::wrapError;

namespace redengine {
namespace text {

	Encoder16OutputStream::Encoder16OutputStream(OutputStream<char>& output, Encoder16& encoder)
			: output(output), encoder(encoder) {}

	Encoder16OutputStream::Encoder16OutputStream(const Encoder16OutputStream& stream)
			: Stream(stream), OutputStream<Char16>(stream), output(stream.output), encoder(stream.encoder) {}

	void Encoder16OutputStream::close() {
		try {
			encoder.endEncoding();
		}
		catch(const RenditionError& error) {
			wrapError<RenditionError, RenditionIOError>(error);
		}
		output.close();
	}

	void Encoder16OutputStream::writeBlock(const Char16* buffer, size_t bufferSize) {
		char byteBuffer[REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE];
		size_t outcount, consumed;
		while(bufferSize) {
			consumed = encoder.encodeBlock(buffer, bufferSize, byteBuffer,
					static_cast<size_t>(REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE), outcount);
			output.write(byteBuffer, outcount);
			buffer += consumed;
			bufferSize -= consumed;
		}
	}

}}
