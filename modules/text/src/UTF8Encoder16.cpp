#include "UTF8Encoder16.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace text {

	UTF8Encoder16::UTF8Encoder16() : pending(0u) {}

	UTF8Encoder16::UTF8Encoder16(const UTF8Encoder16& encoder)
			: Encoder16(encoder), pending(encoder.pending), partial(encoder.partial) {}

	MemorySize UTF8Encoder16::encodeBlock(const Char16* input, MemorySize insize, char* output, MemorySize outsize,
			MemorySize& outcount) {
		unsigned char* out = reinterpret_cast<unsigned char*>(output);
		outcount = static_cast<MemorySize>(0u);
		MemorySize consumed = static_cast<MemorySize>(0u);
		while(consumed < insize && outcount < outsize) {
			if(pending) {
				out[outcount++] = static_cast<unsigned char>((partial & 0x00EFu) | 0x80u);
				partial = static_cast<Char16>(partial >> 6);
				--pending;
			}
			else {
				unsigned c = static_cast<unsigned>(input[consumed++]);
				if(c < 0x0080u)
					out[outcount++] = static_cast<unsigned char>(c);
				else if(c < 0x0800u) {
					partial = static_cast<Char16>(c & 0x003Fu);
					pending = 1u;
					out[outcount++] = static_cast<unsigned char>((c >> 6) | 0xC0u);
				}
				else {
					partial = static_cast<Char16>(c & 0x0FFFu);
					pending = 2u;
					out[outcount++] = static_cast<unsigned char>((c >> 12) | 0xE0u);
				}
			}
		}
		return consumed;
	}

}}
