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

		void reset() {
			state = ST_NONE;
		}

		virtual char getInverseBreakChar(TargetT breakChar) const {
			Char32 c = static_cast<Char32>(breakChar);
			if(c > static_cast<Char32>(0x001FFFFFul))
				throw UnrepresentableCharacterError(c);
			if(c < static_cast<Char32>(0x0080u))
				return static_cast<char>(static_cast<unsigned char>(c));
			if(c < static_cast<Char32>(0x0800u))
				return static_cast<char>(static_cast<unsigned char>((c & static_cast<Char32>(0x003Fu))
						| static_cast<Char32>(0x80u)));
			if(c < static_cast<Char32>(0x00010000ul))
				return static_cast<char>(static_cast<unsigned char>(((c & static_cast<Char32>(0x0FFFu)) >> 6)
						| static_cast<Char32>(0x80u)));
			return static_cast<char>(static_cast<unsigned char>(((c & static_cast<Char32>(0x0003FFFFul)) >> 12)
					| static_cast<Char32>(0x80u)));
		}

		virtual util::MemorySize transcodeBlock(const char* input, util::MemorySize insize,
				TargetT* output, util::MemorySize outsize, util::MemorySize& outcount) {
			const unsigned char* in = reinterpret_cast<const unsigned char*>(input);
			outcount = static_cast<util::MemorySize>(0u);
			util::MemorySize consumed = static_cast<util::MemorySize>(0u);
			for(; consumed < insize && outcount < outsize; ++consumed) {
				unsigned c = static_cast<unsigned>(in[consumed]);
				switch(state) {
					case ST_NONE:
						if(!(c & 0x80u)) {
							TargetT oc = static_cast<TargetT>(c & 0x7Fu);
							output[outcount++] = oc;
							if(oc == this->breakChar)
								insize = static_cast<util::MemorySize>(0u);
						}
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
							if(static_cast<TargetT>(partial) == this->breakChar)
								insize = static_cast<util::MemorySize>(0u);
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

		static util::MemorySize decodeSingleChar(const char* input, util::MemorySize insize, TargetT& output) {
			const unsigned char* in = reinterpret_cast<const unsigned char*>(input);
			util::MemorySize consumed = static_cast<util::MemorySize>(0u);
			bool complete = false;
			State state = ST_NONE;
			for(; !complete && consumed < insize; ++consumed) {
				unsigned c = static_cast<unsigned>(in[consumed]);
				switch(state) {
					case ST_NONE:
						if(!(c & 0x80u)) {
							output = static_cast<TargetT>(c & 0x7Fu);
							complete = true;
						}
						else if((c & 0xE0u) == 0xC0u) {
							output = static_cast<Char32>(c & 0x1Fu);
							state = ST_SEQ2BYTE0;
						}
						else if((c & 0xF0u) == 0xE0u) {
							output = static_cast<Char32>(c & 0x0Fu);
							state = ST_SEQ3BYTE0;
						}
						else if((c & 0xF8u) == 0xF0u) {
							output = static_cast<Char32>(c & 0x07u);
							state = ST_SEQ4BYTE0;
						}
						else
							throw IllegalCodeError();
						break;
					case ST_SEQ3BYTE0:
					case ST_SEQ4BYTE0:
					case ST_SEQ4BYTE1:
						if((c & 0xC0u) == 0x80u) {
							output = static_cast<Char32>((output << 6) | static_cast<Char32>(c & 0x3Fu));
							state = static_cast<State>(static_cast<int>(state) + 1);
						}
						else
							throw IllegalCodeError();
						break;
					case ST_SEQ2BYTE0:
					case ST_SEQ3BYTE1:
					case ST_SEQ4BYTE2:
						if((c & 0xC0u) == 0x80u) {
							output = static_cast<Char32>((output << 6) | static_cast<Char32>(c & 0x3Fu));
							if(util::AddressSpace<TargetT, Char32>::exceededBy(output))
								throw UnrepresentableCharacterError(output);
							complete = true;
						}
						else
							throw IllegalCodeError();
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

	};

	typedef UTF8Decoder<Char16> UTF8Decoder16;
	typedef UTF8Decoder<Char32> UTF8Decoder32;

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF8DECODER_HPP */
