#include <cstring>
#include <redstrain/platform/Endianness.hpp>
#include <redstrain/error/IllegalByteOrderError.hpp>

#include "UTF16Encoder8.hpp"
#include "IllegalCodeError.hpp"
#include "UnrepresentableCharacterError.hpp"

using redengine::util::ByteOrder;
using redengine::util::MemorySize;
using redengine::error::IllegalByteOrderError;
using redengine::util::BO_BIG_ENDIAN;
using redengine::util::BO_LITTLE_ENDIAN;
using redengine::util::BO_AUTODETECT_ENDIANNESS;

namespace redengine {
namespace text {

	UTF16Encoder8::UTF16Encoder8() : byteOrder(BO_AUTODETECT_ENDIANNESS), flags(0),
			bufferFill(static_cast<MemorySize>(0u)), bufferOffset(static_cast<MemorySize>(0u)) {}

	UTF16Encoder8::UTF16Encoder8(const UTF16Encoder8& encoder)
			: TextCodec<Char32, char>(encoder), byteOrder(encoder.byteOrder), flags(encoder.flags),
			codec3216(encoder.codec3216), bufferFill(encoder.bufferFill), bufferOffset(encoder.bufferOffset) {
		if(bufferOffset < bufferFill)
			memcpy(buffer.buffer8 + bufferOffset, encoder.buffer.buffer8 + bufferOffset,
					static_cast<size_t>(bufferFill - bufferOffset));
	}

	UTF16Encoder8::~UTF16Encoder8() {}

	void UTF16Encoder8::setByteOrder(ByteOrder byteOrder) {
		switch(byteOrder) {
			case BO_BIG_ENDIAN:
			case BO_LITTLE_ENDIAN:
			case BO_AUTODETECT_ENDIANNESS:
				this->byteOrder = byteOrder;
				break;
			default:
				throw IllegalByteOrderError(byteOrder);
		}
	}

	void UTF16Encoder8::reset() {
		flags &= ~UTF16Encoder8::IFL_MASK;
		bufferFill = bufferOffset = static_cast<MemorySize>(0u);
		codec3216.reset();
	}

	Char32 UTF16Encoder8::getInverseBreakChar(char breakChar) const {
		throw UnrepresentableCharacterError(static_cast<Char32>(static_cast<unsigned char>(breakChar)));
	}

	MemorySize UTF16Encoder8::transcodeBlock(const Char32* input, MemorySize insize,
			char* output, MemorySize outsize, MemorySize& outcount) {
		if(!(flags & UTF16Encoder8::IFL_BOM_EMITTED)) {
			if(byteOrder != BO_AUTODETECT_ENDIANNESS && !(flags & UTF16Encoder8::FL_SUPPRESS_BOM)) {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_BIG_ENDIAN
				if(byteOrder == BO_LITTLE_ENDIAN)
					*buffer.buffer16 = REDSTRAIN_PLATFORM_SWAB16(0xFEFFu);
				else
					*buffer.buffer16 = static_cast<Char16>(0xFEFFu);
#else /* little endian platform */
				if(byteOrder == BO_LITTLE_ENDIAN)
					*buffer.buffer16 = static_cast<Char16>(0xFEFFu);
				else
					*buffer.buffer16 = REDSTRAIN_PLATFORM_SWAB16(0xFEFFu);
#endif /* little endian platform */
				bufferFill = static_cast<MemorySize>(2u);
			}
			flags |= UTF16Encoder8::IFL_BOM_EMITTED;
		}
		if(bufferOffset < bufferFill) {
			outcount = bufferFill - bufferOffset;
			if(outsize < outcount)
				outcount = outsize;
			if(outcount) {
				memcpy(output, buffer.buffer8, static_cast<size_t>(outcount));
				bufferOffset += outcount;
			}
			return static_cast<MemorySize>(0u);
		}
		MemorySize consumed, tmpcount, u;
		consumed = codec3216.transcodeBlock(input, insize,
				buffer.buffer16, static_cast<MemorySize>(REDSTRAIN_TEXT_UTF16ENCODER8_BUFFER_SIZE), tmpcount);
		bufferOffset = static_cast<MemorySize>(0u);
		bufferFill = tmpcount * static_cast<MemorySize>(2u);
		if(byteOrder == BO_LITTLE_ENDIAN) {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_BIG_ENDIAN
			for(u = static_cast<MemorySize>(0u); u < tmpcount; ++u)
				buffer.buffer16[u] = REDSTRAIN_PLATFORM_SWAB16(buffer.buffer16[u]);
#endif /* big endian platform */
		}
		else {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_LITTLE_ENDIAN
			for(u = static_cast<MemorySize>(0u); u < tmpcount; ++u)
				buffer.buffer16[u] = REDSTRAIN_PLATFORM_SWAB16(buffer.buffer16[u]);
#endif /* little endian platform */
		}
		outcount = bufferFill;
		if(outsize < outcount)
			outcount = outsize;
		if(outcount) {
			memcpy(output, buffer.buffer8, static_cast<size_t>(outcount));
			bufferOffset += outcount;
		}
		return consumed;
	}

	void UTF16Encoder8::endCodeUnit() {
		if(!(flags & UTF16Encoder8::IFL_BOM_EMITTED) && byteOrder != BO_AUTODETECT_ENDIANNESS
				&& !(flags & UTF16Encoder8::FL_SUPPRESS_BOM))
			throw IllegalCodeError();
		if(bufferOffset < bufferFill)
			throw IllegalCodeError();
	}

	MemorySize UTF16Encoder8::encodeSingleChar(Char32 input, char* output, ByteOrder byteOrder) {
		union {
			char buffer8[4];
			Char16 buffer16[2];
		} buffer;
		MemorySize pairs = UTF16Encoder16::encodeSingleChar(input, buffer.buffer16), u;
		if(byteOrder == BO_LITTLE_ENDIAN) {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_BIG_ENDIAN
			for(u = static_cast<MemorySize>(0u); u < pairs; ++u)
				buffer.buffer16[u] = REDSTRAIN_PLATFORM_SWAB16(buffer.buffer16[u]);
#endif /* big endian platform */
		}
		else {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_LITTLE_ENDIAN
			for(u = static_cast<MemorySize>(0u); u < pairs; ++u)
				buffer.buffer16[u] = REDSTRAIN_PLATFORM_SWAB16(buffer.buffer16[u]);
#endif /* little endian platform */
		}
		MemorySize bytes = pairs * static_cast<MemorySize>(2u);
		memcpy(output, buffer.buffer8, static_cast<size_t>(bytes));
		return bytes;
	}

}}
