#ifndef REDSTRAIN_MOD_PROTOSTR_PROTOCOLREADER_HPP
#define REDSTRAIN_MOD_PROTOSTR_PROTOCOLREADER_HPP

#include <string>
#include <stdint.h>
#include <redstrain/io/InputStream.hpp>

#include "api.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace protostr {

	class REDSTRAIN_PROTOSTR_API ProtocolReader {

	  private:
		static const size_t BUFFER_SIZE = static_cast<size_t>(REDSTRAIN_PROTOSTR_READER_BUFFER_SIZE);
		static const size_t STRING_BUFFER_SIZE = static_cast<size_t>(REDSTRAIN_PROTOSTR_READER_STRING_BUFFER_SIZE);

	  private:
		enum State {
			STATE_NONE,
			STATE_HAVE_BLOCK8_SIZE,
			STATE_HAVE_BLOCK16_SIZE,
			STATE_HAVE_BLOCK32_SIZE,
			STATE_SIZED_STREAM,
			STATE_READING_UNSIZED_STREAM8_CHUNK_SIZE,
			STATE_READING_UNSIZED_STREAM16_CHUNK_SIZE,
			STATE_READING_UNSIZED_STREAM32_CHUNK_SIZE,
			STATE_READING_UNSIZED_STREAM8_CHUNK,
			STATE_READING_UNSIZED_STREAM16_CHUNK,
			STATE_READING_UNSIZED_STREAM32_CHUNK
		};

	  private:
		io::InputStream<char>& stream;
		char gatherBuffer[BUFFER_SIZE];
		size_t gatherFill, pendingSize, pendingChunkSize;
		State state;

	  private:
		void readRawElement(size_t);
		void readBlockBody(char*, size_t, size_t&, size_t&);

	  public:
		ProtocolReader(io::InputStream<char>&);
		ProtocolReader(const ProtocolReader&);

		inline io::InputStream<char>& getInputStream() {
			return stream;
		}

		inline const io::InputStream<char>& getInputStream() const {
			return stream;
		}

		int8_t readInt8();
		uint8_t readUInt8();
		int16_t readInt16();
		uint16_t readUInt16();
		int32_t readInt32();
		uint32_t readUInt32();
		int64_t readInt64();
		uint64_t readUInt64();
		float readFloat32();
		double readFloat64();

		int16_t readInt16LE();
		uint16_t readUInt16LE();
		int32_t readInt32LE();
		uint32_t readUInt32LE();
		int64_t readInt64LE();
		uint64_t readUInt64LE();

		int16_t readInt16NBO();
		uint16_t readUInt16NBO();
		int32_t readInt32NBO();
		uint32_t readUInt32NBO();
		int64_t readInt64NBO();
		uint64_t readUInt64NBO();

		void readBlock8(char*, size_t, size_t&, size_t&);
		void readBlock16(char*, size_t, size_t&, size_t&);
		void readBlock32(char*, size_t, size_t&, size_t&);
		void readString8(std::string&);
		void readString16(std::string&);
		void readString32(std::string&);

		void readSizedStream8();
		void readSizedStream16();
		void readSizedStream32();
		void readUnsizedStream8();
		void readUnsizedStream16();
		void readUnsizedStream32();
		size_t readChunk(char*, size_t);
		void readEndOfStream();

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_PROTOCOLREADER_HPP */
