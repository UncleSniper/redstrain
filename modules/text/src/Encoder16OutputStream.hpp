#ifndef REDSTRAIN_MOD_TEXT_ENCODER16OUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_TEXT_ENCODER16OUTPUTSTREAM_HPP

#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"
#include "types.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	class Encoder16;

	class REDSTRAIN_TEXT_API Encoder16OutputStream : public io::OutputStream<Char16> {

	  private:
		io::OutputStream<char>& output;
		Encoder16& encoder;

	  protected:
		Encoder16OutputStream(const Encoder16OutputStream&);

		virtual void writeBlock(const Char16*, size_t);

	  public:
		Encoder16OutputStream(io::OutputStream<char>&, Encoder16&);

		inline io::OutputStream<char>& getBackingOutputStream() {
			return output;
		}

		inline const io::OutputStream<char>& getBackingOutputStream() const {
			return output;
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

#endif /* REDSTRAIN_MOD_TEXT_ENCODER16OUTPUTSTREAM_HPP */
