#ifndef REDSTRAIN_MOD_COMPRESS_COMPRESSINGINPUTSTREAM_HPP
#define REDSTRAIN_MOD_COMPRESS_COMPRESSINGINPUTSTREAM_HPP

#include <redstrain/io/InputStream.hpp>

#include "api.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace compress {

	class Compressor;

	class REDSTRAIN_COMPRESS_API CompressingInputStream : public io::InputStream<char> {

	  private:
		io::InputStream<char>& input;
		Compressor& compressor;
		char buffer[REDSTRAIN_COMPRESS_STREAM_BUFFER_SIZE];
		util::MemorySize bufferFill, bufferOffset;

	  protected:
		CompressingInputStream(const CompressingInputStream&);

		virtual util::MemorySize readBlock(char*, util::MemorySize);

	  public:
		CompressingInputStream(io::InputStream<char>&, Compressor&);

		inline io::InputStream<char>& getBackingInputStream() {
			return input;
		}

		inline const io::InputStream<char>& getBackingInputStream() const {
			return input;
		}

		inline Compressor& getCompressor() {
			return compressor;
		}

		inline const Compressor& getCompressor() const {
			return compressor;
		}

		virtual void close();

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_COMPRESSINGINPUTSTREAM_HPP */
