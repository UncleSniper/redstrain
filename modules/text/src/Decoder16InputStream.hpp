#ifndef REDSTRAIN_MOD_TEXT_DECODER16INPUTSTREAM_HPP
#define REDSTRAIN_MOD_TEXT_DECODER16INPUTSTREAM_HPP

#include <redstrain/io/InputStream.hpp>

#include "api.hpp"
#include "types.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	class Decoder16;

	class REDSTRAIN_TEXT_API Decoder16InputStream : public io::InputStream<Char16> {

	  private:
		io::InputStream<char>& input;
		Decoder16& decoder;
		char byteBuffer[REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE];
		util::MemorySize bufferFill, bufferOffset;

	  protected:
		Decoder16InputStream(const Decoder16InputStream&);

		virtual util::MemorySize readBlock(Char16*, util::MemorySize);

	  public:
		Decoder16InputStream(io::InputStream<char>&, Decoder16&);

		inline InputStream<char>& getBackingInputStream() {
			return input;
		}

		inline const InputStream<char>& getBackingInputStream() const {
			return input;
		}

		inline Decoder16& getDecoder() {
			return decoder;
		}

		inline const Decoder16& getDecoder() const {
			return decoder;
		}

		virtual void close();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DECODER16INPUTSTREAM_HPP */
