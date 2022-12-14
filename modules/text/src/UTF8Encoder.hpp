#ifndef REDSTRAIN_MOD_TEXT_UTF8ENCODER_HPP
#define REDSTRAIN_MOD_TEXT_UTF8ENCODER_HPP

#include "TextCodec.hpp"
#include "IllegalCodeError.hpp"
#include "UnrepresentableCharacterError.hpp"

namespace redengine {
namespace text {

	template<typename SourceT>
	class UTF8Encoder : public TextCodec<SourceT, char> {

	  private:
		unsigned pending;
		Char32 partial;

	  public:
		UTF8Encoder() : pending(0u) {}

		UTF8Encoder(const UTF8Encoder& encoder)
				: TextCodec<SourceT, char>(encoder), pending(encoder.pending), partial(encoder.partial) {}

		void reset() {
			pending = 0u;
		}

		virtual SourceT getInverseBreakChar(char breakChar) const {
			unsigned c = static_cast<unsigned>(static_cast<unsigned char>(breakChar));
			if(c >> 7)
				throw UnrepresentableCharacterError(static_cast<Char32>(c));
			return static_cast<SourceT>(c);
		}

		virtual util::MemorySize transcodeBlock(const SourceT* input, util::MemorySize insize,
				char* output, util::MemorySize outsize, util::MemorySize& outcount) {
			unsigned char* out = reinterpret_cast<unsigned char*>(output);
			outcount = static_cast<util::MemorySize>(0u);
			util::MemorySize consumed = static_cast<util::MemorySize>(0u);
			while((pending || consumed < insize) && outcount < outsize) {
				if(pending) {
					out[outcount++] = static_cast<unsigned char>(static_cast<Char32>(partial
							& static_cast<Char32>(0x003Fu)) | static_cast<Char32>(0x80u));
					partial >>= 6;
					--pending;
				}
				else {
					Char32 c = static_cast<Char32>(input[consumed++]);
					if(c > static_cast<Char32>(0x001FFFFFul))
						throw UnrepresentableCharacterError(c);
					if(c < static_cast<Char32>(0x0080u)) {
						unsigned char uc = static_cast<unsigned char>(c);
						out[outcount++] = uc;
						if(this->breakChar == static_cast<char>(uc))
							break;
					}
					else if(c < static_cast<Char32>(0x0800u)) {
						partial = static_cast<Char32>(c & static_cast<Char32>(0x003Fu));
						pending = 1u;
						out[outcount++] = static_cast<unsigned char>((c >> 6) | static_cast<Char32>(0xC0u));
					}
					else if(c < static_cast<Char32>(0x00010000ul)) {
						partial = static_cast<Char32>(c & static_cast<Char32>(0x0FFFu));
						pending = 2u;
						out[outcount++] = static_cast<unsigned char>((c >> 12) | static_cast<Char32>(0xE0u));
					}
					else {
						partial = static_cast<Char32>(c & static_cast<Char32>(0x0003FFFFul));
						pending = 3u;
						out[outcount++] = static_cast<unsigned char>((c >> 18) | static_cast<Char32>(0xFCu));
					}
				}
			}
			return consumed;
		}

		virtual void endCodeUnit() {
			if(pending)
				throw IllegalCodeError();
		}

		static util::MemorySize encodeSingleChar(SourceT input, char* output) {
			Char32 c = static_cast<Char32>(input);
			unsigned char* out = reinterpret_cast<unsigned char*>(output);
			if(c > static_cast<Char32>(0x001FFFFFul))
				throw UnrepresentableCharacterError(c);
			if(c < static_cast<Char32>(0x0080u)) {
				*out = static_cast<unsigned char>(c);
				return static_cast<util::MemorySize>(1u);
			}
			if(c < static_cast<Char32>(0x0800u)) {
				*out = static_cast<unsigned char>((c >> 6) | static_cast<Char32>(0xC0u));
				c &= static_cast<Char32>(0x003Fu);
				out[1] = static_cast<unsigned char>(static_cast<Char32>(c & static_cast<Char32>(0x003Fu))
						| static_cast<Char32>(0x80u));
				return static_cast<util::MemorySize>(2u);
			}
			if(c < static_cast<Char32>(0x00010000ul)) {
				*out = static_cast<unsigned char>((c >> 12) | static_cast<Char32>(0xE0u));
				c &= static_cast<Char32>(0x0FFFu);
				out[1] = static_cast<unsigned char>(static_cast<Char32>(c & static_cast<Char32>(0x003Fu))
						| static_cast<Char32>(0x80u));
				c >>= 6;
				out[2] = static_cast<unsigned char>(static_cast<Char32>(c & static_cast<Char32>(0x003Fu))
						| static_cast<Char32>(0x80u));
				return static_cast<util::MemorySize>(3u);
			}
			*out = static_cast<unsigned char>((c >> 18) | static_cast<Char32>(0xFCu));
			c &= static_cast<Char32>(0x0003FFFFul);
			out[1] = static_cast<unsigned char>(static_cast<Char32>(c & static_cast<Char32>(0x003Fu))
					| static_cast<Char32>(0x80u));
			c >>= 6;
			out[2] = static_cast<unsigned char>(static_cast<Char32>(c & static_cast<Char32>(0x003Fu))
					| static_cast<Char32>(0x80u));
			c >>= 6;
			out[3] = static_cast<unsigned char>(static_cast<Char32>(c & static_cast<Char32>(0x003Fu))
					| static_cast<Char32>(0x80u));
			return static_cast<util::MemorySize>(4u);
		}

	};

	typedef UTF8Encoder<Char16> UTF8Encoder16;
	typedef UTF8Encoder<Char32> UTF8Encoder32;

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF8ENCODER_HPP */
