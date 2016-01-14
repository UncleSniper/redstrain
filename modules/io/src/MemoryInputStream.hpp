#ifndef REDSTRAIN_MOD_IO_MEMORYINPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_MEMORYINPUTSTREAM_HPP

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
		const RecordT* data;
		util::MemorySize dataSize, readIndex;

	  protected:
		virtual util::MemorySize readBlock(RecordT* buffer, util::MemorySize bufferSize) {
			util::MemorySize restSize = dataSize - readIndex;
			if(bufferSize < restSize)
				restSize = bufferSize;
			if(!restSize)
				return static_cast<util::MemorySize>(0u);
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
		MemoryInputStream(const RecordT* data, util::MemorySize dataSize) : data(data), dataSize(dataSize),
				readIndex(static_cast<util::MemorySize>(0u)) {}
		MemoryInputStream(const MemoryInputStream& stream) : Stream(stream), InputStream<RecordT>(stream),
				data(stream.data), dataSize(stream.dataSize), readIndex(stream.readIndex) {}

		inline RecordT* getBuffer() const {
			return data;
		}

		inline util::MemorySize getBufferSize() const {
			return dataSize;
		}

		virtual util::FileSize tell() const {
			return static_cast<util::FileSize>(readIndex);
		}

		virtual void seek(util::FileOffset offset, Stream::SeekWhence whence) {
			util::FileOffset target;
			switch(whence) {
				case Stream::OFFSET_FROM_START:
					target = offset;
					break;
				case Stream::OFFSET_FROM_HERE:
					target = static_cast<util::FileOffset>(readIndex) + offset;
					break;
				case Stream::OFFSET_FROM_END:
					target = static_cast<util::FileOffset>(dataSize) + offset;
					break;
				default:
					throw IllegalSeekWhenceError(whence);
			}
			if(target < static_cast<util::FileOffset>(0)
					|| static_cast<util::FileSize>(target) > static_cast<util::FileSize>(dataSize))
				throw SeekOffsetOutOfBoundsError(target);
			readIndex = static_cast<util::MemorySize>(target);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_MEMORYINPUTSTREAM_HPP */
