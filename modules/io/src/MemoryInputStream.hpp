#ifndef REDSTRAIN_MOD_IO_MEMORYINPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_MEMORYINPUTSTREAM_HPP

#include <redstrain/util/StringUtils.hpp>

#include "InputStream.hpp"
#include "IllegalSeekWhenceError.hpp"
#include "SeekOffsetOutOfBoundsError.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class MemoryInputStream : public InputStream<RecordT> {

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
		const RecordT* data;
		size_t dataSize, readIndex;

	  protected:
		virtual size_t readBlock(RecordT* buffer, size_t bufferSize) {
			size_t restSize = dataSize - readIndex;
			if(bufferSize < restSize)
				restSize = bufferSize;
			if(!restSize)
				return static_cast<size_t>(0u);
			DestroyRecords destroy(buffer);
			const RecordT *src, *end;
			src = data + readIndex;
			end = src + restSize;
			for(; src != end; ++src, ++buffer) {
				new(buffer) RecordT(*src);
				++destroy.count;
				++readIndex;
			}
			destroy.release();
			return restSize;
		}

	  public:
		MemoryInputStream(const RecordT* data, size_t dataSize) : data(data), dataSize(dataSize),
				readIndex(static_cast<size_t>(0u)) {}
		MemoryInputStream(const MemoryInputStream& stream) : Stream(stream), InputStream<RecordT>(stream),
				data(stream.data), dataSize(stream.dataSize), readIndex(stream.readIndex) {}

		inline RecordT* getBuffer() const {
			return data;
		}

		inline size_t getBufferSize() const {
			return dataSize;
		}

		virtual size_t tell() const {
			return readIndex;
		}

		virtual void seek(off_t offset, Stream::SeekWhence whence) {
			off_t target;
			switch(whence) {
				case Stream::OFFSET_FROM_START:
					target = offset;
					break;
				case Stream::OFFSET_FROM_HERE:
					target = static_cast<off_t>(readIndex) + offset;
					break;
				case Stream::OFFSET_FROM_END:
					target = static_cast<off_t>(dataSize) + offset;
					break;
				default:
					throw IllegalSeekWhenceError(whence);
			}
			if(target < static_cast<off_t>(0) || static_cast<size_t>(target) > dataSize)
				throw SeekOffsetOutOfBoundsError(target);
			readIndex = static_cast<size_t>(target);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_MEMORYINPUTSTREAM_HPP */
