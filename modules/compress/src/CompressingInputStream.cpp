#include <cstring>
#include <redstrain/error/handler.hpp>

#include "Compressor.hpp"
#include "CompressionIOError.hpp"
#include "CompressingInputStream.hpp"

using redengine::io::InputStream;
using redengine::util::MemorySize;
using redengine::error::wrapError;

namespace redengine {
namespace compress {

	CompressingInputStream::CompressingInputStream(InputStream<char>& input, Compressor& compressor)
			: input(input), compressor(compressor), bufferFill(static_cast<MemorySize>(0u)),
			bufferOffset(static_cast<MemorySize>(0u)) {}

	CompressingInputStream::CompressingInputStream(const CompressingInputStream& stream)
			: Stream(stream), InputStream<char>(stream), input(stream.input), compressor(stream.compressor),
			bufferFill(stream.bufferFill), bufferOffset(stream.bufferOffset) {
		if(bufferOffset < bufferFill)
			memcpy(buffer + bufferOffset, stream.buffer + bufferOffset,
					static_cast<size_t>(bufferFill - bufferOffset));
	}

	MemorySize CompressingInputStream::readBlock(char* outBuffer, MemorySize outBufferSize) {
		if(!outBufferSize)
			return static_cast<MemorySize>(0u);
		MemorySize outcount, consumed;
		do {
			if(bufferOffset >= bufferFill) {
				MemorySize count = input.read(buffer, static_cast<MemorySize>(REDSTRAIN_COMPRESS_STREAM_BUFFER_SIZE));
				if(!count) {
					try {
						compressor.compressBlock(NULL, static_cast<MemorySize>(0u),
								outBuffer, outBufferSize, outcount);
						if(outcount)
							return outcount;
						atEnd = true;
						compressor.endCompression();
					}
					catch(const CompressionError& error) {
						wrapError<CompressionError, CompressionIOError>(error);
					}
					return static_cast<MemorySize>(0u);
				}
				bufferFill = count;
				bufferOffset = static_cast<MemorySize>(0u);
			}
			try {
				consumed = compressor.compressBlock(buffer + bufferOffset, bufferFill - bufferOffset,
						outBuffer, outBufferSize, outcount);
			}
			catch(const CompressionError& error) {
				wrapError<CompressionError, CompressionIOError>(error);
			}
			bufferOffset += consumed;
		} while(!outcount);
		return outcount;
	}

	void CompressingInputStream::close() {
		input.close();
	}

}}
