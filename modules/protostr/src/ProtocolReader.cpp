#include <cstring>
#include <redstrain/platform/Endianness.hpp>

#include "ProtocolReader.hpp"
#include "IllegalReaderStateError.hpp"
#include "AnnouncedSizeMismatchError.hpp"
#include "UnexpectedEndOfStreamError.hpp"

using std::string;
using redengine::util::FileSize;
using redengine::io::InputStream;
using redengine::util::MemorySize;
using redengine::platform::Endianness;

namespace redengine {
namespace protostr {

	ProtocolReader::ProtocolReader(InputStream<char>& stream) : stream(stream), state(STATE_NONE) {}

	ProtocolReader::ProtocolReader(const ProtocolReader& reader)
			: stream(reader.stream), gatherFill(reader.gatherFill), pendingSize(reader.pendingSize),
			pendingChunkSize(reader.pendingChunkSize), state(reader.state) {
		if(gatherFill)
			memcpy(gatherBuffer, reader.gatherBuffer, static_cast<size_t>(gatherFill));
	}

	void ProtocolReader::readRawElement(MemorySize size) {
		while(gatherFill < size) {
			MemorySize count = stream.read(gatherBuffer + gatherFill, size - gatherFill);
			if(!count)
				throw UnexpectedEndOfStreamError();
			gatherFill += count;
		}
	}

#define ensureClean \
	do { \
		if(state != STATE_NONE) \
			throw IllegalReaderStateError(); \
	} while(0)
#define gatherInto(var) \
	do { \
		gatherFill = static_cast<MemorySize>(0u); \
		readRawElement(static_cast<MemorySize>(sizeof(var))); \
		memcpy(&var, gatherBuffer, sizeof(var)); \
	} while(0)
#define fromBig(var, type) (var = Endianness<type>::convertBig(var))
#define fromLittle(var, type) (var = Endianness<type>::convertLittle(var))
#define usePending(size, nextState) \
	do { \
		pendingSize = static_cast<FileSize>(size); \
		state = STATE_ ## nextState; \
	} while(0)

	int8_t ProtocolReader::readInt8() {
		int8_t value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	uint8_t ProtocolReader::readUInt8() {
		uint8_t value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	int16_t ProtocolReader::readInt16() {
		int16_t value;
		ensureClean;
		gatherInto(value);
		fromBig(value, int16_t);
		return value;
	}

	uint16_t ProtocolReader::readUInt16() {
		uint16_t value;
		ensureClean;
		gatherInto(value);
		fromBig(value, uint16_t);
		return value;
	}

	int32_t ProtocolReader::readInt32() {
		int32_t value;
		ensureClean;
		gatherInto(value);
		fromBig(value, int32_t);
		return value;
	}

	uint32_t ProtocolReader::readUInt32() {
		uint32_t value;
		ensureClean;
		gatherInto(value);
		fromBig(value, uint32_t);
		return value;
	}

	int64_t ProtocolReader::readInt64() {
		int64_t value;
		ensureClean;
		gatherInto(value);
		fromBig(value, int64_t);
		return value;
	}

	uint64_t ProtocolReader::readUInt64() {
		uint64_t value;
		ensureClean;
		gatherInto(value);
		fromBig(value, uint64_t);
		return value;
	}

	float ProtocolReader::readFloat32() {
		float value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	double ProtocolReader::readFloat64() {
		double value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	int16_t ProtocolReader::readInt16LE() {
		int16_t value;
		ensureClean;
		gatherInto(value);
		fromLittle(value, int16_t);
		return value;
	}

	uint16_t ProtocolReader::readUInt16LE() {
		uint16_t value;
		ensureClean;
		gatherInto(value);
		fromLittle(value, uint16_t);
		return value;
	}

	int32_t ProtocolReader::readInt32LE() {
		int32_t value;
		ensureClean;
		gatherInto(value);
		fromLittle(value, int32_t);
		return value;
	}

	uint32_t ProtocolReader::readUInt32LE() {
		uint32_t value;
		ensureClean;
		gatherInto(value);
		fromLittle(value, uint32_t);
		return value;
	}

	int64_t ProtocolReader::readInt64LE() {
		int64_t value;
		ensureClean;
		gatherInto(value);
		fromLittle(value, int64_t);
		return value;
	}

	uint64_t ProtocolReader::readUInt64LE() {
		uint64_t value;
		ensureClean;
		gatherInto(value);
		fromLittle(value, uint64_t);
		return value;
	}

	int16_t ProtocolReader::readInt16NBO() {
		int16_t value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	uint16_t ProtocolReader::readUInt16NBO() {
		uint16_t value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	int32_t ProtocolReader::readInt32NBO() {
		int32_t value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	uint32_t ProtocolReader::readUInt32NBO() {
		uint32_t value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	int64_t ProtocolReader::readInt64NBO() {
		int64_t value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	uint64_t ProtocolReader::readUInt64NBO() {
		uint64_t value;
		ensureClean;
		gatherInto(value);
		return value;
	}

	void ProtocolReader::readBlockBody(char* buffer, MemorySize bufferSize, MemorySize& chunkSize,
			FileSize& remainingSize) {
		if(static_cast<FileSize>(bufferSize) > pendingSize)
			bufferSize = static_cast<MemorySize>(pendingSize);
		if(!bufferSize) {
			chunkSize = static_cast<MemorySize>(0u);
			remainingSize = pendingSize;
			return;
		}
		chunkSize = stream.read(buffer, bufferSize);
		pendingSize -= static_cast<FileSize>(chunkSize);
		remainingSize = pendingSize;
	}

	void ProtocolReader::readBlock8(char* buffer, MemorySize bufferSize, MemorySize& chunkSize,
			FileSize& remainingSize) {
		uint8_t wireSize;
		switch(state) {
			case STATE_NONE:
				gatherInto(wireSize);
				usePending(wireSize, HAVE_BLOCK8_SIZE);
			case STATE_HAVE_BLOCK8_SIZE:
				readBlockBody(buffer, bufferSize, chunkSize, remainingSize);
				break;
			default:
				throw IllegalReaderStateError();
		}
	}

	void ProtocolReader::readBlock16(char* buffer, MemorySize bufferSize, MemorySize& chunkSize,
			FileSize& remainingSize) {
		uint16_t wireSize;
		switch(state) {
			case STATE_NONE:
				gatherInto(wireSize);
				usePending(wireSize, HAVE_BLOCK16_SIZE);
			case STATE_HAVE_BLOCK16_SIZE:
				readBlockBody(buffer, bufferSize, chunkSize, remainingSize);
				break;
			default:
				throw IllegalReaderStateError();
		}
	}

	void ProtocolReader::readBlock32(char* buffer, MemorySize bufferSize, MemorySize& chunkSize,
			FileSize& remainingSize) {
		uint32_t wireSize;
		switch(state) {
			case STATE_NONE:
				gatherInto(wireSize);
				usePending(wireSize, HAVE_BLOCK32_SIZE);
			case STATE_HAVE_BLOCK32_SIZE:
				readBlockBody(buffer, bufferSize, chunkSize, remainingSize);
				break;
			default:
				throw IllegalReaderStateError();
		}
	}

	void ProtocolReader::readString8(string& buffer) {
		char byteBuffer[STRING_BUFFER_SIZE];
		MemorySize chunkSize;
		FileSize remainingSize;
		do {
			readBlock8(byteBuffer, STRING_BUFFER_SIZE, chunkSize, remainingSize);
			buffer.append(byteBuffer, static_cast<string::size_type>(chunkSize));
		} while(remainingSize);
	}

	void ProtocolReader::readString16(string& buffer) {
		char byteBuffer[STRING_BUFFER_SIZE];
		MemorySize chunkSize;
		FileSize remainingSize;
		do {
			readBlock16(byteBuffer, STRING_BUFFER_SIZE, chunkSize, remainingSize);
			buffer.append(byteBuffer, static_cast<string::size_type>(chunkSize));
		} while(remainingSize);
	}

	void ProtocolReader::readString32(string& buffer) {
		char byteBuffer[STRING_BUFFER_SIZE];
		MemorySize chunkSize;
		FileSize remainingSize;
		do {
			readBlock32(byteBuffer, STRING_BUFFER_SIZE, chunkSize, remainingSize);
			buffer.append(byteBuffer, static_cast<string::size_type>(chunkSize));
		} while(remainingSize);
	}

	void ProtocolReader::readSizedStream8() {
		uint8_t wireSize;
		ensureClean;
		gatherInto(wireSize);
		usePending(wireSize, SIZED_STREAM);
	}

	void ProtocolReader::readSizedStream16() {
		uint16_t wireSize;
		ensureClean;
		gatherInto(wireSize);
		usePending(wireSize, SIZED_STREAM);
	}

	void ProtocolReader::readSizedStream32() {
		uint32_t wireSize;
		ensureClean;
		gatherInto(wireSize);
		usePending(wireSize, SIZED_STREAM);
	}

	void ProtocolReader::readUnsizedStream8() {
		ensureClean;
		state = STATE_READING_UNSIZED_STREAM8_CHUNK_SIZE;
	}

	void ProtocolReader::readUnsizedStream16() {
		ensureClean;
		state = STATE_READING_UNSIZED_STREAM16_CHUNK_SIZE;
	}

	void ProtocolReader::readUnsizedStream32() {
		ensureClean;
		state = STATE_READING_UNSIZED_STREAM32_CHUNK_SIZE;
	}

	MemorySize ProtocolReader::readChunk(char* buffer, MemorySize bufferSize) {
		MemorySize chunkSize;
		FileSize remainingSize;
		uint8_t wireSize8;
		uint16_t wireSize16;
		uint32_t wireSize32;
		switch(state) {
			case STATE_SIZED_STREAM:
				readBlockBody(buffer, bufferSize, chunkSize, remainingSize);
				return chunkSize;
			case STATE_READING_UNSIZED_STREAM8_CHUNK_SIZE:
				gatherInto(wireSize8);
				usePending(wireSize8, READING_UNSIZED_STREAM8_CHUNK);
				if(!pendingSize) {
					state = STATE_READING_UNSIZED_STREAM8_CHUNK_SIZE;
					return static_cast<MemorySize>(0u);
				}
			case STATE_READING_UNSIZED_STREAM8_CHUNK:
				readBlockBody(buffer, bufferSize, chunkSize, remainingSize);
				if(!remainingSize)
					state = STATE_READING_UNSIZED_STREAM8_CHUNK_SIZE;
				return chunkSize;
			case STATE_READING_UNSIZED_STREAM16_CHUNK_SIZE:
				gatherInto(wireSize16);
				fromBig(wireSize16, uint16_t);
				usePending(wireSize16, READING_UNSIZED_STREAM16_CHUNK);
				if(!pendingSize) {
					state = STATE_READING_UNSIZED_STREAM16_CHUNK_SIZE;
					return static_cast<MemorySize>(0u);
				}
			case STATE_READING_UNSIZED_STREAM16_CHUNK:
				readBlockBody(buffer, bufferSize, chunkSize, remainingSize);
				if(!remainingSize)
					state = STATE_READING_UNSIZED_STREAM16_CHUNK_SIZE;
				return chunkSize;
			case STATE_READING_UNSIZED_STREAM32_CHUNK_SIZE:
				gatherInto(wireSize32);
				fromBig(wireSize32, uint32_t);
				usePending(wireSize32, READING_UNSIZED_STREAM32_CHUNK);
				if(!pendingSize) {
					state = STATE_READING_UNSIZED_STREAM32_CHUNK_SIZE;
					return static_cast<MemorySize>(0u);
				}
			case STATE_READING_UNSIZED_STREAM32_CHUNK:
				readBlockBody(buffer, bufferSize, chunkSize, remainingSize);
				if(!remainingSize)
					state = STATE_READING_UNSIZED_STREAM32_CHUNK_SIZE;
				return chunkSize;
			default:
				throw IllegalReaderStateError();
		}
	}

	void ProtocolReader::readEndOfStream() {
		switch(state) {
			case STATE_SIZED_STREAM:
				if(pendingSize)
					throw AnnouncedSizeMismatchError();
				break;
			case STATE_READING_UNSIZED_STREAM8_CHUNK_SIZE:
			case STATE_READING_UNSIZED_STREAM16_CHUNK_SIZE:
			case STATE_READING_UNSIZED_STREAM32_CHUNK_SIZE:
				break;
			default:
				throw IllegalReaderStateError();
		}
		state = STATE_NONE;
	}

}}
