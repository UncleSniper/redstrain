#include <cstring>
#include <redstrain/platform/Endianness.hpp>
#include <redstrain/error/IllegalByteOrderError.hpp>

#include "UTF16Decoder8.hpp"
#include "IllegalCodeError.hpp"

using redengine::util::ByteOrder;
using redengine::util::MemorySize;
using redengine::error::IllegalByteOrderError;
using redengine::util::BO_BIG_ENDIAN;
using redengine::util::BO_LITTLE_ENDIAN;
using redengine::util::BO_AUTODETECT_ENDIANNESS;

namespace redengine {
namespace text {

	UTF16Decoder8::UTF16Decoder8() : byteOrder(BO_AUTODETECT_ENDIANNESS), defaultByteOrder(BO_BIG_ENDIAN), flags(0),
			bufferFill(static_cast<MemorySize>(0u)), bufferOffset(static_cast<MemorySize>(0u)) {}

	UTF16Decoder8::UTF16Decoder8(const UTF16Decoder8& decoder)
			: TextCodec<char, Char32>(decoder), byteOrder(decoder.byteOrder),
			defaultByteOrder(decoder.defaultByteOrder), flags(decoder.flags), codec1632(decoder.codec1632),
			bufferFill(decoder.bufferFill), bufferOffset(decoder.bufferOffset) {
		if(bufferOffset < bufferFill)
			memcpy(buffer.buffer8 + bufferOffset, decoder.buffer.buffer8 + bufferOffset,
					static_cast<size_t>(bufferFill - bufferOffset));
	}

	UTF16Decoder8::~UTF16Decoder8() {}

	void UTF16Decoder8::setByteOrder(ByteOrder byteOrder) {
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

	void UTF16Decoder8::setDefaultByteOrder(ByteOrder defaultByteOrder) {
		switch(defaultByteOrder) {
			case BO_BIG_ENDIAN:
			case BO_LITTLE_ENDIAN:
			case BO_AUTODETECT_ENDIANNESS:
				this->defaultByteOrder = defaultByteOrder;
				break;
			default:
				throw IllegalByteOrderError(defaultByteOrder);
		}
	}

	MemorySize UTF16Decoder8::transcodeBlock(const char* input, MemorySize insize,
			Char32* output, MemorySize outsize, MemorySize& outcount) {
		// invariant: bufferOffset is always even
		MemorySize have = bufferFill - bufferOffset, consumed;
		if(have < static_cast<MemorySize>(2u)) {
			if(have && bufferOffset) {
				*buffer.buffer8 = buffer.buffer8[bufferOffset];
				bufferOffset = static_cast<MemorySize>(0u);
				bufferFill = static_cast<MemorySize>(1u);
			}
			else
				bufferOffset = bufferFill = static_cast<MemorySize>(0u);
			consumed = static_cast<MemorySize>(REDSTRAIN_TEXT_UTF16DECODER8_BUFFER_SIZE * 2u) - bufferFill;
			if(insize < consumed)
				consumed = insize;
			memcpy(buffer.buffer8 + bufferFill, input, static_cast<size_t>(consumed));
			bufferFill += consumed;
			have += consumed;
			MemorySize u, swabPairs = have / static_cast<MemorySize>(2u);
			if(swabPairs && byteOrder == BO_AUTODETECT_ENDIANNESS && !(flags & UTF16Decoder8::IFL_BOM_READ)) {
				Char16 bom = *buffer.buffer16;
				if(bom == static_cast<Char16>(0xFEFFu)) {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_BIG_ENDIAN
					byteOrder = BO_BIG_ENDIAN;
#else /* little endian platform */
					byteOrder = BO_LITTLE_ENDIAN;
#endif /* little endian platform */
					bufferOffset += static_cast<MemorySize>(2u);
				}
				else if(bom == static_cast<Char16>(0xFFFEu)) {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_BIG_ENDIAN
					byteOrder = BO_LITTLE_ENDIAN;
#else /* little endian platform */
					byteOrder = BO_BIG_ENDIAN;
#endif /* little endian platform */
					bufferOffset += static_cast<MemorySize>(2u);
				}
				else
					byteOrder = defaultByteOrder == BO_LITTLE_ENDIAN ? BO_LITTLE_ENDIAN : BO_BIG_ENDIAN;
			}
			if(byteOrder == BO_LITTLE_ENDIAN) {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_BIG_ENDIAN
				for(u = static_cast<MemorySize>(0u); u < swabPairs; ++u)
					buffer.buffer16[u] = REDSTRAIN_PLATFORM_SWAB16(buffer.buffer16[u]);
#endif /* big endian platform */
			}
			else {
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_LITTLE_ENDIAN
				for(u = static_cast<MemorySize>(0u); u < swabPairs; ++u)
					buffer.buffer16[u] = REDSTRAIN_PLATFORM_SWAB16(buffer.buffer16[u]);
#endif /* little endian platform */
			}
		}
		else
			consumed = static_cast<MemorySize>(0u);
		MemorySize c = codec1632.transcodeBlock(buffer.buffer16 + bufferOffset / static_cast<MemorySize>(2u),
				have / static_cast<MemorySize>(2u), output, outsize, outcount);
		bufferOffset += c * static_cast<MemorySize>(2u);
		return consumed;
	}

	void UTF16Decoder8::endCodeUnit() {
		if(bufferOffset < bufferFill)
			throw IllegalCodeError();
	}

}}
