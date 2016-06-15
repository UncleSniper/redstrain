#ifndef REDSTRAIN_MOD_PROTOSTR_PROTOCOLWRITER_HPP
#define REDSTRAIN_MOD_PROTOSTR_PROTOCOLWRITER_HPP

#include <string>
#include <stdint.h>
#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace protostr {

	class REDSTRAIN_PROTOSTR_API ProtocolWriter {

	  public:
		static const util::MemorySize BUFFER_SIZE
				= static_cast<util::MemorySize>(REDSTRAIN_PROTOSTR_WRITER_BUFFER_SIZE);

	  private:
		enum State {
			STATE_NONE,
			STATE_HAVE_BLOCK_SIZE,
			STATE_SIZED_STREAM,
			STATE_UNSIZED_STREAM8,
			STATE_UNSIZED_STREAM16,
			STATE_UNSIZED_STREAM32,
			STATE_HAVE_UNSIZED_STREAM8_SIZE,
			STATE_HAVE_UNSIZED_STREAM16_SIZE,
			STATE_HAVE_UNSIZED_STREAM32_SIZE
		};

	  private:
		io::OutputStream<char>& stream;
		char gatherBuffer[BUFFER_SIZE];
		util::MemorySize gatherFill;
		util::FileSize pendingSize;
		State state;

	  public:
		ProtocolWriter(io::OutputStream<char>&);
		ProtocolWriter(const ProtocolWriter&);

		inline io::OutputStream<char>& getOutputStream() {
			return stream;
		}

		inline const io::OutputStream<char>& getOutputStream() const {
			return stream;
		}

		inline util::MemorySize getBufferFill() const {
			return gatherFill;
		}

		void writeRawElement(const char*, util::MemorySize);
		void flush();

		void writeInt8(int8_t);
		void writeUInt8(uint8_t);
		void writeInt16(int16_t);
		void writeUInt16(uint16_t);
		void writeInt32(int32_t);
		void writeUInt32(uint32_t);
		void writeInt64(int64_t);
		void writeUInt64(uint64_t);
		void writeFloat32(float);
		void writeFloat64(double);

		void writeInt8LE(int8_t);
		void writeUInt8LE(uint8_t);
		void writeInt16LE(int16_t);
		void writeUInt16LE(uint16_t);
		void writeInt32LE(int32_t);
		void writeUInt32LE(uint32_t);
		void writeInt64LE(int64_t);
		void writeUInt64LE(uint64_t);
		void writeFloat32LE(float);
		void writeFloat64LE(double);

		void writeInt8NBO(int8_t);
		void writeUInt8NBO(uint8_t);
		void writeInt16NBO(int16_t);
		void writeUInt16NBO(uint16_t);
		void writeInt32NBO(int32_t);
		void writeUInt32NBO(uint32_t);
		void writeInt64NBO(int64_t);
		void writeUInt64NBO(uint64_t);
		void writeFloat32NBO(float);
		void writeFloat64NBO(double);

		void writeBlock8(const char*, util::MemorySize);
		void writeBlock16(const char*, util::MemorySize);
		void writeBlock32(const char*, util::MemorySize);
		void writeString8(const std::string&);
		void writeString16(const std::string&);
		void writeString32(const std::string&);

		void writeSizedStream8(util::FileSize);
		void writeSizedStream16(util::FileSize);
		void writeSizedStream32(util::FileSize);
		void writeUnsizedStream8();
		void writeUnsizedStream16();
		void writeUnsizedStream32();
		void writeChunk(const char*, util::MemorySize);
		void writeEndOfStream();

	};

	template<typename PrimitiveT>
	void writeProtocolPrimitive(ProtocolWriter&, PrimitiveT);

	template<typename PrimitiveT>
	void writeProtocolPrimitiveLE(ProtocolWriter&, PrimitiveT);

	template<typename PrimitiveT>
	void writeProtocolPrimitiveNBO(ProtocolWriter&, PrimitiveT);

#define REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(type, function) \
	template<> \
	inline void writeProtocolPrimitive<type>(ProtocolWriter& writer, type value) { \
		writer.write ## function(value); \
	} \
	template<> \
	inline void writeProtocolPrimitiveLE<type>(ProtocolWriter& writer, type value) { \
		writer.write ## function ## LE(value); \
	} \
	template<> \
	inline void writeProtocolPrimitiveNBO<type>(ProtocolWriter& writer, type value) { \
		writer.write ## function ## NBO(value); \
	}

	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(int8_t, Int8)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(uint8_t, UInt8)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(int16_t, Int16)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(uint16_t, UInt16)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(int32_t, Int32)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(uint32_t, UInt32)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(int64_t, Int64)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(uint64_t, UInt64)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(float, Float32)
	REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE(double, Float64)

#undef REDSTRAIN_PROTOSTR_DEFINE_PRIMITIVE

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_PROTOCOLWRITER_HPP */
