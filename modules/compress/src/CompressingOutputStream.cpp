#include <redstrain/error/handler.hpp>

#include "Compressor.hpp"
#include "CompressionIOError.hpp"
#include "CompressingOutputStream.hpp"
#include "tweaks.hpp"

using redengine::io::OutputStream;
using redengine::util::MemorySize;
using redengine::error::wrapError;

namespace redengine {
namespace compress {

	CompressingOutputStream::CompressingOutputStream(OutputStream<char>& output, Compressor& compressor)
			: output(output), compressor(compressor) {}

	CompressingOutputStream::CompressingOutputStream(const CompressingOutputStream& stream)
			: Stream(stream), OutputStream<char>(stream), output(stream.output), compressor(stream.compressor) {}

	void CompressingOutputStream::writeBlock(const char* buffer, MemorySize bufferSize) {
		char targetBuffer[REDSTRAIN_COMPRESS_STREAM_BUFFER_SIZE];
		MemorySize outcount, consumed;
		while(bufferSize) {
			try {
				consumed = compressor.compressBlock(buffer, bufferSize, targetBuffer,
						static_cast<MemorySize>(REDSTRAIN_COMPRESS_STREAM_BUFFER_SIZE), outcount);
			}
			catch(const CompressionError& error) {
				wrapError<CompressionError, CompressionIOError>(error);
			}
			output.write(targetBuffer, outcount);
			buffer += consumed;
			bufferSize -= consumed;
		}
	}

	void CompressingOutputStream::flushCompression() {
		char targetBuffer[REDSTRAIN_COMPRESS_STREAM_BUFFER_SIZE];
		MemorySize outcount;
		try {
			for(;;) {
				compressor.compressBlock(NULL, static_cast<MemorySize>(0u), targetBuffer,
						static_cast<MemorySize>(REDSTRAIN_COMPRESS_STREAM_BUFFER_SIZE), outcount);
				if(!outcount)
					break;
				output.write(targetBuffer, outcount);
			}
			compressor.endCompression();
		}
		catch(const CompressionError& error) {
			wrapError<CompressionError, CompressionIOError>(error);
		}
	}

	void CompressingOutputStream::close() {
		flushCompression();
		output.close();
	}

}}
