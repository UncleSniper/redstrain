#include "UTF16Decoder16.hpp"
#include "IllegalCodeError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace text {

	UTF16Decoder16::UTF16Decoder16() : state(ST_NONE) {}

	UTF16Decoder16::UTF16Decoder16(const UTF16Decoder16& decoder)
			: TextCodec<Char16, Char32>(decoder), state(decoder.state), partial(decoder.partial) {}

	MemorySize UTF16Decoder16::transcodeBlock(const Char16* input, MemorySize insize,
			Char32* output, MemorySize outsize, MemorySize& outcount) {
		outcount = static_cast<MemorySize>(0u);
		MemorySize consumed = static_cast<MemorySize>(0u);
		for(; consumed < insize && outcount < outsize; ++consumed) {
			unsigned c = static_cast<unsigned>(input[consumed]);
			switch(state) {
				case ST_NONE:
					if((c & 0xF800u) == 0xD800u) {
						if(c & 0x0400u) {
							state = ST_ERROR;
							throw IllegalCodeError();
						}
						partial = static_cast<Char16>(c & 0x07FFu);
						state = ST_SPUNIT0;
					}
					else
						output[outcount++] = static_cast<Char32>(c);
					break;
				case ST_SPUNIT0:
					if((c & 0xFC00u) != 0xDC00u) {
						state = ST_ERROR;
						throw IllegalCodeError();
					}
					output[outcount++] = static_cast<Char32>(static_cast<Char32>(
							(static_cast<Char32>(partial) << 10) | static_cast<Char32>(c & 0x03FFu))
							+ static_cast<Char32>(0x010000u));
					state = ST_NONE;
					break;
				case ST_ERROR:
				default:
					throw IllegalCodeError();
			}
		}
		return consumed;
	}

	void UTF16Decoder16::endCodeUnit() {
		if(state != ST_NONE)
			throw IllegalCodeError();
	}

}}
