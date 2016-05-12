#include "UTF16Decoder16.hpp"
#include "IllegalCodeError.hpp"
#include "UnrepresentableCharacterError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace text {

	UTF16Decoder16::UTF16Decoder16() : state(ST_NONE) {}

	UTF16Decoder16::UTF16Decoder16(const UTF16Decoder16& decoder)
			: TextCodec<Char16, Char32>(decoder), state(decoder.state), partial(decoder.partial) {}

	void UTF16Decoder16::reset() {
		state = ST_NONE;
	}

	Char16 UTF16Decoder16::getInverseBreakChar(Char32 breakChar) const {
		if(breakChar >> 16) {
			if(breakChar > static_cast<Char32>(0x0010FFFFul))
				throw UnrepresentableCharacterError(breakChar);
			breakChar -= static_cast<Char32>(0x00010000ul);
			return static_cast<Char16>((breakChar & static_cast<Char32>(0x03FFu)) | static_cast<Char32>(0xDC00u));
		}
		if((breakChar & static_cast<Char32>(0xF800u)) == static_cast<Char32>(0xD800u))
			throw UnrepresentableCharacterError(breakChar);
		return static_cast<Char16>(breakChar);
	}

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
					else {
						output[outcount++] = static_cast<Char32>(c);
						if(this->breakChar == static_cast<Char32>(c))
							insize = static_cast<MemorySize>(0u);
					}
					break;
				case ST_SPUNIT0:
					if((c & 0xFC00u) != 0xDC00u) {
						state = ST_ERROR;
						throw IllegalCodeError();
					}
					{
						Char32 c32 = static_cast<Char32>(static_cast<Char32>(
								(static_cast<Char32>(partial) << 10) | static_cast<Char32>(c & 0x03FFu))
								+ static_cast<Char32>(0x010000u));
						output[outcount++] = c32;
						state = ST_NONE;
						if(c32 == this->breakChar)
							insize = static_cast<MemorySize>(0u);
					}
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

	MemorySize UTF16Decoder16::decodeSingleChar(const Char16* input, MemorySize insize, Char32& output) {
		MemorySize consumed = static_cast<MemorySize>(0u);
		bool complete = false;
		State state = ST_NONE;
		for(; !complete && consumed < insize; ++consumed) {
			unsigned c = static_cast<unsigned>(input[consumed]);
			switch(state) {
				case ST_NONE:
					if((c & 0xF800u) == 0xD800u) {
						if(c & 0x0400u)
							throw IllegalCodeError();
						output = static_cast<Char32>(c & 0x07FFu);
						state = ST_SPUNIT0;
					}
					else {
						output = static_cast<Char32>(c);
						complete = true;
					}
					break;
				case ST_SPUNIT0:
					if((c & 0xFC00u) != 0xDC00u)
						throw IllegalCodeError();
					output = static_cast<Char32>(static_cast<Char32>((output << 10)
							| static_cast<Char32>(c & 0x03FFu)) + static_cast<Char32>(0x010000u));
					complete = true;
					break;
				case ST_ERROR:
				default:
					throw IllegalCodeError();
			}
		}
		if(!complete)
			throw IllegalCodeError();
		return consumed;
	}

}}
