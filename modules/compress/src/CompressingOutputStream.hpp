#ifndef REDSTRAIN_MOD_COMPRESS_COMPRESSINGOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_COMPRESS_COMPRESSINGOUTPUTSTREAM_HPP

#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"

namespace redengine {
namespace compress {

	class Compressor;

	class REDSTRAIN_COMPRESS_API CompressingOutputStream : public io::OutputStream<char> {

	  private:
		io::OutputStream<char>& output;
		Compressor& compressor;

	  protected:
		CompressingOutputStream(const CompressingOutputStream&);

		virtual void writeBlock(const char*, util::MemorySize);

	  public:
		CompressingOutputStream(io::OutputStream<char>&, Compressor&);

		inline io::OutputStream<char>& getBackingOutputStream() {
			return output;
		}

		inline const io::OutputStream<char>& getBackingOutputStream() const {
			return output;
		}

		inline Compressor& getCompressor() {
			return compressor;
		}

		inline const Compressor& getCompressor() const {
			return compressor;
		}

		void flushCompression();

		virtual void close();

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_COMPRESSINGOUTPUTSTREAM_HPP */
