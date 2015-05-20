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
			size_t count;

		  public:
			DestroyRecords(RecordT* buffer) : buffer(buffer), count(static_cast<size_t>(0u)) {}

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
		size_t dataSize, readIndex, writeIndex;

	  private:
		FixedBufferStream(const FixedBufferStream& stream)
				: Stream(stream), InputStream<RecordT>(stream), OutputStream<RecordT>(stream) {}

	  protected:
		virtual size_t readBlock(RecordT* buffer, size_t bufferSize) {
			size_t restSize = getDataSize();
			if(bufferSize < restSize)
				restSize = bufferSize;
			if(!restSize)
				return static_cast<size_t>(0u);
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
				size_t half = dataSize - readIndex;
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

		virtual void writeBlock(const RecordT* buffer, size_t count) {
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
				size_t half = dataSize - writeIndex;
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
		FixedBufferStream(RecordT* data, size_t dataSize)
				: data(data), dataSize(dataSize), readIndex(static_cast<size_t>(0u)),
				writeIndex(static_cast<size_t>(0u)) {}

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

		inline size_t getBufferSize() const {
			return dataSize;
		}

		inline size_t getDataSize() const {
			return dataSize ? (writeIndex + dataSize - readIndex) % dataSize : static_cast<size_t>(0u);
		}

		inline size_t getFreeRecords() const {
			return dataSize ? dataSize - (writeIndex + dataSize - readIndex) % dataSize : static_cast<size_t>(0u);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_FIXEDBUFFERSTREAM_HPP */
