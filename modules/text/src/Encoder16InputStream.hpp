#ifndef REDSTRAIN_MOD_TEXT_ENCODER16INPUTSTREAM_HPP
#define REDSTRAIN_MOD_TEXT_ENCODER16INPUTSTREAM_HPP

#include <redstrain/io/InputStream.hpp>

#include "api.hpp"
#include "types.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	class Encoder16;

	class REDSTRAIN_TEXT_API Encoder16InputStream : public io::InputStream<char> {

	  private:
		io::InputStream<Char16>& input;
		Encoder16& encoder;
		Char16 charBuffer[REDSTRAIN_TEXT_CODEC_STREAM_BUFFER_SIZE];
		size_t bufferFill, bufferOffset;

	  protected:
		Encoder16InputStream(const Encoder16InputStream&);

		virtual size_t readBlock(char*, size_t);

	  public:
		Encoder16InputStream(io::InputStream<Char16>&, Encoder16&);

		inline InputStream<Char16>& getBackingInputStream() {
			return input;
		}

		inline const InputStream<Char16>& getBackingInputStream() const {
			return input;
		}

		inline Encoder16& getEncoder() {
			return encoder;
		}

		inline const Encoder16& getEncoder() const {
			return encoder;
		}

		virtual void close();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_ENCODER16INPUTSTREAM_HPP */
