#include "UTF16Encoder16.hpp"
#include "IllegalCodeError.hpp"
#include "UnrepresentableCharacterError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace text {

	UTF16Encoder16::UTF16Encoder16() : pending(0u) {}

	UTF16Encoder16::UTF16Encoder16(const UTF16Encoder16& encoder)
			: TextCodec<Char32, Char16>(encoder), pending(encoder.pending), partial(encoder.partial) {}

	void UTF16Encoder16::reset() {
		pending = 0u;
	}

	Char32 UTF16Encoder16::getInverseBreakChar(Char16 breakChar) const {
		unsigned c = static_cast<unsigned>(breakChar);
		if((c & 0xF800u) == 0xD800u)
				throw UnrepresentableCharacterError(static_cast<Char32>(c));
		return static_cast<Char32>(c);
	}

	MemorySize UTF16Encoder16::transcodeBlock(const Char32* input, MemorySize insize,
			Char16* output, MemorySize outsize, MemorySize& outcount) {
		outcount = static_cast<MemorySize>(0u);
		MemorySize consumed = static_cast<MemorySize>(0u);
		while((pending || consumed < insize) && outcount < outsize) {
			if(pending) {
				output[outcount++] = static_cast<Char16>(partial | static_cast<Char16>(0xDC00u));
				--pending;
			}
			else {
				Char32 c = input[consumed++];
				if(c >> 16) {
					if(c > static_cast<Char32>(0x0010FFFFul))
						throw UnrepresentableCharacterError(c);
					c -= static_cast<Char32>(0x00010000ul);
					output[outcount++] = static_cast<Char16>(static_cast<Char16>(c >> 10)
							| static_cast<Char16>(0xD800u));
					partial = static_cast<Char16>(c & static_cast<Char32>(0x000003FFul));
					pending = 1u;
				}
				else {
					if((c & static_cast<Char32>(0xF800u)) == static_cast<Char32>(0xD800u))
						throw UnrepresentableCharacterError(c);
					output[outcount++] = static_cast<Char16>(c);
					if(this->breakChar == static_cast<Char16>(c))
						break;
				}
			}
		}
		return consumed;
	}

	void UTF16Encoder16::endCodeUnit() {
		if(pending)
			throw IllegalCodeError();
	}

	MemorySize UTF16Encoder16::encodeSingleChar(Char32 input, Char16* output) {
		if(input >> 16) {
			if(input > static_cast<Char32>(0x0010FFFFul))
				throw UnrepresentableCharacterError(input);
			input -= static_cast<Char32>(0x00010000ul);
			*output = static_cast<Char16>(static_cast<Char16>(input >> 10) | static_cast<Char16>(0xD800u));
			output[1] = static_cast<Char16>(static_cast<Char16>(input & static_cast<Char32>(0x000003FFul))
					| static_cast<Char16>(0xDC00u));
			return static_cast<MemorySize>(2u);
		}
		else {
			if((input & static_cast<Char32>(0xF800u)) == static_cast<Char32>(0xD800u))
				throw UnrepresentableCharacterError(input);
			*output = static_cast<Char16>(input);
			return static_cast<MemorySize>(1u);
		}
	}

}}
