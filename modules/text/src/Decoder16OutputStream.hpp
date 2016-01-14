#ifndef REDSTRAIN_MOD_TEXT_DECODER16OUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_TEXT_DECODER16OUTPUTSTREAM_HPP

#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"
#include "types.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	class Decoder16;

	class REDSTRAIN_TEXT_API Decoder16OutputStream : public io::OutputStream<char> {

	  private:
		io::OutputStream<Char16>& output;
		Decoder16& decoder;

	  protected:
		Decoder16OutputStream(const Decoder16OutputStream&);

		virtual void writeBlock(const char*, util::MemorySize);

	  public:
		Decoder16OutputStream(io::OutputStream<Char16>&, Decoder16&);

		inline io::OutputStream<Char16>& getBackingOutputStream() {
			return output;
		}

		inline const io::OutputStream<Char16>& getBackingOutputStream() const {
			return output;
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

#endif /* REDSTRAIN_MOD_TEXT_DECODER16OUTPUTSTREAM_HPP */
