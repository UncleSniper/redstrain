#include <redstrain/error/handler.hpp>

#include "Decoder16.hpp"
#include "RenditionIOError.hpp"
#include "Decoder16OutputStream.hpp"

using redengine::io::OutputStream;
using redengine::util::MemorySize;
using redengine::error::wrapError;

namespace redengine {
namespace text {

	Decoder16OutputStream::Decoder16OutputStream(OutputStream<Char16>& output, Decoder16& decoder)
			: output(output), decoder(decoder) {}

	Decoder16OutputStream::Decoder16OutputStream(const Decoder16OutputStream& stream)
			: Stream(stream), OutputStream<char>(stream), output(stream.output), decoder(stream.decoder) {}

	void Decoder16OutputStream::close() {
		try {
			decoder.endDecoding();
		}
		catch(const RenditionError& error) {
			wrapError<RenditionError, RenditionIOError>(error);
		}
		output.close();
	}

	void Decoder16OutputStream::writeBlock(const char* buffer, MemorySize bufferSize) {
		Char16 charBuffer[REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE];
		MemorySize outcount, consumed;
		while(bufferSize) {
			consumed = decoder.decodeBlock(buffer, bufferSize, charBuffer,
					static_cast<MemorySize>(REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE), outcount);
			output.write(charBuffer, outcount);
			buffer += consumed;
			bufferSize -= consumed;
		}
	}

}}
