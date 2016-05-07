#include "UTF16Encoder16.hpp"
#include "IllegalCodeError.hpp"
#include "UnrepresentableCharacterError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace text {

	UTF16Encoder16::UTF16Encoder16() : pending(0u) {}

	UTF16Encoder16::UTF16Encoder16(const UTF16Encoder16& encoder)
			: TextCodec<Char32, Char16>(encoder), pending(encoder.pending), partial(encoder.partial) {}

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
					partial = static_cast<Char16>(static_cast<Char16>(c & static_cast<Char32>(0x000003FFul)));
					pending = 1u;
				}
				else {
					if((c & static_cast<Char32>(0xF800u)) == static_cast<Char32>(0xD800u))
						throw UnrepresentableCharacterError(c);
					output[outcount++] = static_cast<Char16>(c);
				}
			}
		}
		return consumed;
	}

	void UTF16Encoder16::endCodeUnit() {
		if(pending)
			throw IllegalCodeError();
	}

}}
