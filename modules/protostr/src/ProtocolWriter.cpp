#include <cstring>
#include <redstrain/util/AddressSpace.hpp>
#include <redstrain/platform/FloatEndianness.hpp>

#include "ProtocolWriter.hpp"
#include "IllegalWriterStateError.hpp"
#include "AddressSpaceExceededError.hpp"
#include "AnnouncedSizeMismatchError.hpp"

using std::string;
using redengine::util::FileSize;
using redengine::util::MemorySize;
using redengine::io::OutputStream;
using redengine::util::AddressSpace;
using redengine::platform::Endianness;
using redengine::platform::FloatEndianness;

namespace redengine {
namespace protostr {

	ProtocolWriter::ProtocolWriter(OutputStream<char>& stream) : stream(stream),
			gatherFill(static_cast<MemorySize>(0u)), state(STATE_NONE) {}

	ProtocolWriter::ProtocolWriter(const ProtocolWriter& writer) : stream(writer.stream),
			gatherFill(writer.gatherFill), pendingSize(writer.pendingSize), state(writer.state) {
		if(gatherFill)
			memcpy(gatherBuffer, writer.gatherBuffer, static_cast<size_t>(gatherFill));
	}

	void ProtocolWriter::writeRawElement(const char* element, MemorySize size) {
		if(!size)
			return;
		if(gatherFill + size <= BUFFER_SIZE) {
			memcpy(gatherBuffer + gatherFill, element, static_cast<size_t>(size));
			gatherFill += size;
			return;
		}
		if(gatherFill) {
			stream.write(gatherBuffer, gatherFill);
			gatherFill = static_cast<MemorySize>(0u);
		}
		if(size >= BUFFER_SIZE) {
			stream.write(element, size);
			return;
		}
		memcpy(gatherBuffer, element, static_cast<size_t>(size));
		gatherFill = size;
	}

	void ProtocolWriter::flush() {
		if(!gatherFill)
			return;
		stream.write(gatherBuffer, gatherFill);
		gatherFill = static_cast<MemorySize>(0u);
	}

#define ensureClean \
	do { \
		if(state != STATE_NONE) \
			throw IllegalWriterStateError(); \
	} while(0)
#define writeDirect(var, size) \
	(writeRawElement(reinterpret_cast<const char*>(&var), static_cast<MemorySize>(size ## u)))
#define toBig(type) (value = Endianness<type>::convertBig(value))
#define toLittle(type) (value = Endianness<type>::convertLittle(value))
#define toBigFloat(ivar, fvar, type) \
	typename FloatEndianness<type>::Bits ivar = FloatEndianness<type>::toBigBits(fvar)
#define toLittleFloat(ivar, fvar, type) \
	typename FloatEndianness<type>::Bits ivar = FloatEndianness<type>::toLittleBits(fvar)
#define toNativeFloat(ivar, fvar, type) \
	typename FloatEndianness<type>::Bits ivar = FloatEndianness<type>::toNativeBits(fvar)
#define checkSizeSpace(size, wtype, stype) \
	do { \
		if(AddressSpace<wtype, stype>::exceededBy(size)) \
			throw AddressSpaceExceededError(); \
	} while(0)

	// (big endian) canonical primitives

	void ProtocolWriter::writeInt8(int8_t value) {
		ensureClean;
		writeDirect(value, 1);
	}

	void ProtocolWriter::writeUInt8(uint8_t value) {
		ensureClean;
		writeDirect(value, 1);
	}

	void ProtocolWriter::writeInt16(int16_t value) {
		ensureClean;
		toBig(int16_t);
		writeDirect(value, 2);
	}

	void ProtocolWriter::writeUInt16(uint16_t value) {
		ensureClean;
		toBig(uint16_t);
		writeDirect(value, 2);
	}

	void ProtocolWriter::writeInt32(int32_t value) {
		ensureClean;
		toBig(int32_t);
		writeDirect(value, 4);
	}

	void ProtocolWriter::writeUInt32(uint32_t value) {
		ensureClean;
		toBig(uint32_t);
		writeDirect(value, 4);
	}

	void ProtocolWriter::writeInt64(int64_t value) {
		ensureClean;
		toBig(int64_t);
		writeDirect(value, 8);
	}

	void ProtocolWriter::writeUInt64(uint64_t value) {
		ensureClean;
		toBig(uint64_t);
		writeDirect(value, 8);
	}

	void ProtocolWriter::writeFloat32(float value) {
		ensureClean;
		toBigFloat(bits, value, float);
		writeDirect(bits, 4);
	}

	void ProtocolWriter::writeFloat64(double value) {
		ensureClean;
		toBigFloat(bits, value, double);
		writeDirect(bits, 8);
	}

	// little endian primitives

	void ProtocolWriter::writeInt8LE(int8_t value) {
		ensureClean;
		writeDirect(value, 1);
	}

	void ProtocolWriter::writeUInt8LE(uint8_t value) {
		ensureClean;
		writeDirect(value, 1);
	}

	void ProtocolWriter::writeInt16LE(int16_t value) {
		ensureClean;
		toLittle(int16_t);
		writeDirect(value, 2);
	}

	void ProtocolWriter::writeUInt16LE(uint16_t value) {
		ensureClean;
		toLittle(uint16_t);
		writeDirect(value, 2);
	}

	void ProtocolWriter::writeInt32LE(int32_t value) {
		ensureClean;
		toLittle(int32_t);
		writeDirect(value, 4);
	}

	void ProtocolWriter::writeUInt32LE(uint32_t value) {
		ensureClean;
		toLittle(uint32_t);
		writeDirect(value, 4);
	}

	void ProtocolWriter::writeInt64LE(int64_t value) {
		ensureClean;
		toLittle(int64_t);
		writeDirect(value, 8);
	}

	void ProtocolWriter::writeUInt64LE(uint64_t value) {
		ensureClean;
		toLittle(uint64_t);
		writeDirect(value, 8);
	}

	void ProtocolWriter::writeFloat32LE(float value) {
		ensureClean;
		toLittleFloat(bits, value, float);
		writeDirect(bits, 4);
	}

	void ProtocolWriter::writeFloat64LE(double value) {
		ensureClean;
		toLittleFloat(bits, value, double);
		writeDirect(bits, 8);
	}

	// native byte order primitives

	void ProtocolWriter::writeInt8NBO(int8_t value) {
		ensureClean;
		writeDirect(value, 1);
	}

	void ProtocolWriter::writeUInt8NBO(uint8_t value) {
		ensureClean;
		writeDirect(value, 1);
	}

	void ProtocolWriter::writeInt16NBO(int16_t value) {
		ensureClean;
		writeDirect(value, 2);
	}

	void ProtocolWriter::writeUInt16NBO(uint16_t value) {
		ensureClean;
		writeDirect(value, 2);
	}

	void ProtocolWriter::writeInt32NBO(int32_t value) {
		ensureClean;
		writeDirect(value, 4);
	}

	void ProtocolWriter::writeUInt32NBO(uint32_t value) {
		ensureClean;
		writeDirect(value, 4);
	}

	void ProtocolWriter::writeInt64NBO(int64_t value) {
		ensureClean;
		writeDirect(value, 8);
	}

	void ProtocolWriter::writeUInt64NBO(uint64_t value) {
		ensureClean;
		writeDirect(value, 8);
	}

	void ProtocolWriter::writeFloat32NBO(float value) {
		ensureClean;
		toNativeFloat(bits, value, float);
		writeDirect(bits, 4);
	}

	void ProtocolWriter::writeFloat64NBO(double value) {
		ensureClean;
		toNativeFloat(bits, value, double);
		writeDirect(bits, 8);
	}

	void ProtocolWriter::writeBlock8(const char* data, MemorySize size) {
		uint8_t wireSize;
		switch(state) {
			case STATE_NONE:
				if(!size)
					break;
				checkSizeSpace(size, uint8_t, MemorySize);
				wireSize = static_cast<uint8_t>(size);
				writeDirect(wireSize, 1);
				state = STATE_HAVE_BLOCK_SIZE;
			case STATE_HAVE_BLOCK_SIZE:
				writeRawElement(data, size);
				state = STATE_NONE;
				break;
			default:
				throw IllegalWriterStateError();
		}
	}

	void ProtocolWriter::writeBlock16(const char* data, MemorySize size) {
		uint16_t wireSize;
		switch(state) {
			case STATE_NONE:
				if(!size)
					break;
				checkSizeSpace(size, uint16_t, MemorySize);
				wireSize = Endianness<uint16_t>::convertBig(static_cast<uint16_t>(size));
				writeDirect(wireSize, 2);
				state = STATE_HAVE_BLOCK_SIZE;
			case STATE_HAVE_BLOCK_SIZE:
				writeRawElement(data, size);
				state = STATE_NONE;
				break;
			default:
				throw IllegalWriterStateError();
		}
	}

	void ProtocolWriter::writeBlock32(const char* data, MemorySize size) {
		uint32_t wireSize;
		switch(state) {
			case STATE_NONE:
				if(!size)
					break;
				checkSizeSpace(size, uint32_t, MemorySize);
				wireSize = Endianness<uint32_t>::convertBig(static_cast<uint32_t>(size));
				writeDirect(wireSize, 4);
				state = STATE_HAVE_BLOCK_SIZE;
			case STATE_HAVE_BLOCK_SIZE:
				writeRawElement(data, size);
				state = STATE_NONE;
				break;
			default:
				throw IllegalWriterStateError();
		}
	}

	void ProtocolWriter::writeString8(const string& data) {
		writeBlock8(data.data(), static_cast<MemorySize>(data.length()));
	}

	void ProtocolWriter::writeString16(const string& data) {
		writeBlock16(data.data(), static_cast<MemorySize>(data.length()));
	}

	void ProtocolWriter::writeString32(const string& data) {
		writeBlock32(data.data(), static_cast<MemorySize>(data.length()));
	}

	void ProtocolWriter::writeSizedStream8(FileSize size) {
		ensureClean;
		checkSizeSpace(size, uint8_t, FileSize);
		uint8_t wireSize = static_cast<uint8_t>(size);
		writeDirect(wireSize, 1);
		state = STATE_SIZED_STREAM;
		pendingSize = size;
	}

	void ProtocolWriter::writeSizedStream16(FileSize size) {
		ensureClean;
		checkSizeSpace(size, uint16_t, FileSize);
		uint16_t wireSize = Endianness<uint16_t>::convertBig(static_cast<uint16_t>(size));
		writeDirect(wireSize, 2);
		state = STATE_SIZED_STREAM;
		pendingSize = size;
	}

	void ProtocolWriter::writeSizedStream32(FileSize size) {
		ensureClean;
		checkSizeSpace(size, uint32_t, FileSize);
		uint32_t wireSize = Endianness<uint32_t>::convertBig(static_cast<uint32_t>(size));
		writeDirect(wireSize, 4);
		state = STATE_SIZED_STREAM;
		pendingSize = size;
	}

	void ProtocolWriter::writeUnsizedStream8() {
		ensureClean;
		state = STATE_UNSIZED_STREAM8;
	}

	void ProtocolWriter::writeUnsizedStream16() {
		ensureClean;
		state = STATE_UNSIZED_STREAM16;
	}

	void ProtocolWriter::writeUnsizedStream32() {
		ensureClean;
		state = STATE_UNSIZED_STREAM32;
	}

	void ProtocolWriter::writeChunk(const char* data, MemorySize size) {
		uint8_t wireSize8;
		uint16_t wireSize16;
		uint32_t wireSize32;
		switch(state) {
			case STATE_SIZED_STREAM:
				if(!size)
					break;
				if(static_cast<FileSize>(size) > pendingSize)
					throw AnnouncedSizeMismatchError();
				writeRawElement(data, size);
				pendingSize -= static_cast<FileSize>(size);
				break;
			case STATE_UNSIZED_STREAM8:
				if(!size)
					break;
				checkSizeSpace(size, uint8_t, MemorySize);
				wireSize8 = static_cast<uint8_t>(size);
				writeDirect(wireSize8, 1);
					break;
				state = STATE_HAVE_UNSIZED_STREAM8_SIZE;
			case STATE_HAVE_UNSIZED_STREAM8_SIZE:
				writeRawElement(data, size);
				state = STATE_UNSIZED_STREAM8;
				break;
			case STATE_UNSIZED_STREAM16:
				if(!size)
					break;
				checkSizeSpace(size, uint16_t, MemorySize);
				wireSize16 = Endianness<uint16_t>::convertBig(static_cast<uint16_t>(size));
				writeDirect(wireSize16, 2);
				state = STATE_HAVE_UNSIZED_STREAM16_SIZE;
			case STATE_HAVE_UNSIZED_STREAM16_SIZE:
				writeRawElement(data, size);
				state = STATE_UNSIZED_STREAM16;
				break;
			case STATE_UNSIZED_STREAM32:
				if(!size)
					break;
				checkSizeSpace(size, uint32_t, MemorySize);
				wireSize32 = Endianness<uint32_t>::convertBig(static_cast<uint32_t>(size));
				writeDirect(wireSize32, 4);
				state = STATE_HAVE_UNSIZED_STREAM32_SIZE;
			case STATE_HAVE_UNSIZED_STREAM32_SIZE:
				writeRawElement(data, size);
				state = STATE_UNSIZED_STREAM32;
				break;
			default:
				throw IllegalWriterStateError();
		}
	}

	static const uint32_t ZERO = static_cast<uint32_t>(0u);

	void ProtocolWriter::writeEndOfStream() {
		switch(state) {
			case STATE_SIZED_STREAM:
				if(pendingSize)
					throw AnnouncedSizeMismatchError();
				break;
			case STATE_UNSIZED_STREAM8:
				writeDirect(ZERO, 1);
				break;
			case STATE_UNSIZED_STREAM16:
				writeDirect(ZERO, 2);
				break;
			case STATE_UNSIZED_STREAM32:
				writeDirect(ZERO, 4);
				break;
			default:
				throw IllegalWriterStateError();
		}
		state = STATE_NONE;
	}

}}
