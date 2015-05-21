#include "UTF8Decoder16.hpp"
#include "IllegalCodeError.hpp"

namespace redengine {
namespace text {

	UTF8Decoder16::UTF8Decoder16() : state(ST_NONE) {}

	UTF8Decoder16::UTF8Decoder16(const UTF8Decoder16& decoder)
			: Decoder16(decoder), state(decoder.state), partial(decoder.partial) {}

	size_t UTF8Decoder16::decodeBlock(const char* input, size_t insize, Char16* output, size_t outsize,
			size_t& outcount) {
		const unsigned char* in = reinterpret_cast<const unsigned char*>(input);
		outcount = static_cast<size_t>(0u);
		size_t consumed = static_cast<size_t>(0u);
		for(; consumed < insize && outcount < outsize; ++consumed) {
			unsigned c = static_cast<unsigned>(in[consumed]);
			switch(state) {
				case ST_NONE:
					if(!(c & 0x80u))
						output[outcount++] = static_cast<Char16>(c & 0x7Fu);
					else if((c & 0xE0u) == 0xC0u) {
						partial = static_cast<Char16>(c & 0x1Fu);
						state = ST_SEQ2BYTE0;
					}
					else if((c & 0xF0u) == 0xE0u) {
						partial = static_cast<Char16>(c & 0x0Fu);
						state = ST_SEQ3BYTE0;
					}
					else {
						state = ST_ERROR;
						throw IllegalCodeError();
					}
					break;
				case ST_SEQ3BYTE0:
					if((c & 0xC0u) == 0x80u) {
						partial = static_cast<Char16>((partial << 6) | static_cast<Char16>(c & 0x3Fu));
						state = ST_SEQ3BYTE1;
					}
					else {
						state = ST_ERROR;
						throw IllegalCodeError();
					}
				case ST_SEQ2BYTE0:
				case ST_SEQ3BYTE1:
					if((c & 0xC0u) == 0x80u) {
						output[outcount++] = static_cast<Char16>((partial << 6) | static_cast<Char16>(c & 0x3Fu));
						state = ST_NONE;
					}
					else {
						state = ST_ERROR;
						throw IllegalCodeError();
					}
				default:
					throw IllegalCodeError();
			}
		}
		return consumed;
	}

	void UTF8Decoder16::endDecoding() {
		if(state != ST_NONE)
			throw IllegalCodeError();
	}

}}
