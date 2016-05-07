#ifndef REDSTRAIN_MOD_TEXT_UTF8DECODER_HPP
#define REDSTRAIN_MOD_TEXT_UTF8DECODER_HPP

#include <redstrain/util/AddressSpace.hpp>

#include "TextCodec.hpp"
#include "IllegalCodeError.hpp"
#include "UnrepresentableCharacterError.hpp"

namespace redengine {
namespace text {

	template<typename TargetT>
	class UTF8Decoder : public TextCodec<char, TargetT> {

	  private:
		enum State {
			ST_NONE,
			ST_SEQ2BYTE0,
			ST_SEQ3BYTE0,
			ST_SEQ3BYTE1,
			ST_SEQ4BYTE0,
			ST_SEQ4BYTE1,
			ST_SEQ4BYTE2,
			ST_ERROR
		};

	  private:
		State state;
		Char32 partial;

	  public:
		UTF8Decoder() : state(ST_NONE) {}

		UTF8Decoder(const UTF8Decoder& decoder)
				: TextCodec<char, TargetT>(decoder), state(decoder.state), partial(decoder.partial) {}

		util::MemorySize transcodeBlock(const char* input, util::MemorySize insize,
				TargetT* output, util::MemorySize outsize, util::MemorySize& outcount) {
			const unsigned char* in = reinterpret_cast<const unsigned char*>(input);
			outcount = static_cast<util::MemorySize>(0u);
			util::MemorySize consumed = static_cast<util::MemorySize>(0u);
			for(; consumed < insize && outcount < outsize; ++consumed) {
				unsigned c = static_cast<unsigned>(in[consumed]);
				switch(state) {
					case ST_NONE:
						if(!(c & 0x80u))
							output[outcount++] = static_cast<TargetT>(c & 0x7Fu);
						else if((c & 0xE0u) == 0xC0u) {
							partial = static_cast<Char32>(c & 0x1Fu);
							state = ST_SEQ2BYTE0;
						}
						else if((c & 0xF0u) == 0xE0u) {
							partial = static_cast<Char32>(c & 0x0Fu);
							state = ST_SEQ3BYTE0;
						}
						else if((c & 0xF8u) == 0xF0u) {
							partial = static_cast<Char32>(c & 0x07u);
							state = ST_SEQ4BYTE0;
						}
						else {
							state = ST_ERROR;
							throw IllegalCodeError();
						}
						break;
					case ST_SEQ3BYTE0:
					case ST_SEQ4BYTE0:
					case ST_SEQ4BYTE1:
						if((c & 0xC0u) == 0x80u) {
							partial = static_cast<Char32>((partial << 6) | static_cast<Char32>(c & 0x3Fu));
							state = static_cast<State>(static_cast<int>(state) + 1);
						}
						else {
							state = ST_ERROR;
							throw IllegalCodeError();
						}
						break;
					case ST_SEQ2BYTE0:
					case ST_SEQ3BYTE1:
					case ST_SEQ4BYTE2:
						if((c & 0xC0u) == 0x80u) {
							partial = static_cast<Char32>((partial << 6) | static_cast<Char32>(c & 0x3Fu));
							if(util::AddressSpace<TargetT, Char32>::exceededBy(partial))
								throw UnrepresentableCharacterError(partial);
							output[outcount++] = static_cast<TargetT>(partial);
							state = ST_NONE;
						}
						else {
							state = ST_ERROR;
							throw IllegalCodeError();
						}
						break;
					case ST_ERROR:
					default:
						throw IllegalCodeError();
				}
			}
			return consumed;
		}

		virtual void endCodeUnit() {
			if(state != ST_NONE)
				throw IllegalCodeError();
		}

	};

	typedef UTF8Decoder<Char16> UTF8Decoder16;
	typedef UTF8Decoder<Char32> UTF8Decoder32;

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF8DECODER_HPP */
