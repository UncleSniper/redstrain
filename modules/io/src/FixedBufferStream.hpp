#ifndef REDSTRAIN_MOD_IO_FIXEDBUFFERSTREAM_HPP
#define REDSTRAIN_MOD_IO_FIXEDBUFFERSTREAM_HPP

#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "StreamBufferFullError.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class FixedBufferStream : public InputStream<RecordT>, public OutputStream<RecordT> {

	  private:
		class DestroyRecords {

		  private:
			RecordT* buffer;

		  public:
			util::MemorySize count;

		  public:
			DestroyRecords(RecordT* buffer) : buffer(buffer), count(static_cast<util::MemorySize>(0u)) {}

			~DestroyRecords() {
				if(!buffer)
					return;
				RecordT* end = buffer + count;
				for(; buffer != end; ++buffer)
					buffer->~RecordT();
			}

			void release() {
				buffer = NULL;
			}

		};

	  private:
		RecordT* data;
		util::MemorySize dataSize, readIndex, writeIndex;

	  private:
		FixedBufferStream(const FixedBufferStream& stream)
				: Stream(stream), InputStream<RecordT>(stream), OutputStream<RecordT>(stream),
				data(stream.data), dataSize(stream.dataSize), readIndex(stream.readIndex),
				writeIndex(stream.writeIndex) {}

	  protected:
		virtual util::MemorySize readBlock(RecordT* buffer, util::MemorySize bufferSize) {
			util::MemorySize restSize = getDataSize();
			if(bufferSize < restSize)
				restSize = bufferSize;
			if(!restSize)
				return static_cast<util::MemorySize>(0u);
			const RecordT *src, *end;
			DestroyRecords destroy(buffer);
			if(readIndex + restSize < dataSize) {
				src = data + readIndex;
				end = src + restSize;
				for(; src != end; ++src, ++buffer) {
					new(buffer) RecordT(*src);
					++destroy.count;
					++readIndex;
					readIndex %= dataSize;
				}
			}
			else {
				util::MemorySize half = dataSize - readIndex;
				src = data + readIndex;
				end = src + half;
				for(; src != end; ++src, ++buffer) {
					new(buffer) RecordT(*src);
					++destroy.count;
					++readIndex;
					readIndex %= dataSize;
				}
				src = data;
				end = src + (restSize - half);
				for(; src != end; ++src, ++buffer) {
					new(buffer) RecordT(*src);
					++destroy.count;
					++readIndex;
					readIndex %= dataSize;
				}
			}
			destroy.release();
			return restSize;
		}

		virtual void writeBlock(const RecordT* buffer, util::MemorySize count) {
			if(getFreeRecords() <= count)
				throw StreamBufferFullError();
			RecordT* dest;
			const RecordT* end;
			if(writeIndex + count < dataSize) {
				dest = data + writeIndex;
				end = buffer + count;
				for(; buffer != end; ++buffer, ++dest) {
					new(dest) RecordT(*buffer);
					++writeIndex;
					writeIndex %= dataSize;
				}
			}
			else {
				util::MemorySize half = dataSize - writeIndex;
				dest = data + writeIndex;
				end = buffer + half;
				for(; buffer != end; ++buffer, ++dest) {
					new(dest) RecordT(*buffer);
					++writeIndex;
					writeIndex %= dataSize;
				}
				dest = data;
				end = buffer + (count - half);
				for(; buffer != end; ++buffer, ++dest) {
					new(dest) RecordT(*buffer);
					++writeIndex;
					writeIndex %= dataSize;
				}
			}
		}

	  public:
		FixedBufferStream(RecordT* data, util::MemorySize dataSize)
				: data(data), dataSize(dataSize), readIndex(static_cast<util::MemorySize>(0u)),
				writeIndex(static_cast<util::MemorySize>(0u)) {}

		virtual ~FixedBufferStream() {
			if(dataSize) {
				while(writeIndex != readIndex) {
					data[readIndex].~RecordT();
					++readIndex;
					readIndex %= dataSize;
				}
			}
		}

		inline RecordT* getBuffer() const {
			return data;
		}

		inline util::MemorySize getBufferSize() const {
			return dataSize;
		}

		inline util::MemorySize getDataSize() const {
			return dataSize ? (writeIndex + dataSize - readIndex) % dataSize : static_cast<util::MemorySize>(0u);
		}

		inline util::MemorySize getFreeRecords() const {
			return dataSize ? dataSize - (writeIndex + dataSize - readIndex) % dataSize
					: static_cast<util::MemorySize>(0u);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_FIXEDBUFFERSTREAM_HPP */
