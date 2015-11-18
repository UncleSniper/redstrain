#ifndef REDSTRAIN_MOD_IO_MEMORYOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_MEMORYOUTPUTSTREAM_HPP

#include <redstrain/util/StringUtils.hpp>

#include "OutputStream.hpp"
#include "StreamBufferFullError.hpp"
#include "IllegalSeekWhenceError.hpp"
#include "SeekOffsetOutOfBoundsError.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class MemoryOutputStream : public OutputStream<RecordT> {

	  private:
		RecordT* data;
		size_t dataSize, writeIndex;

	  protected:
		virtual void writeBlock(const RecordT* buffer, size_t count) {
			size_t restSize = dataSize - writeIndex;
			if(restSize <= count)
				throw StreamBufferFullError();
			RecordT* dest = data + writeIndex;
			const RecordT* end = buffer + count;
			for(; buffer != end; ++buffer, ++dest) {
				new(dest) RecordT(*buffer);
				++writeIndex;
			}
		}

	  public:
		MemoryOutputStream(RecordT* data, size_t dataSize) : data(data), dataSize(dataSize),
				writeIndex(static_cast<size_t>(0u)) {}
		MemoryOutputStream(const MemoryOutputStream& stream) : Stream(stream), OutputStream<RecordT>(stream),
				data(stream.data), dataSize(stream.dataSize), writeIndex(stream.writeIndex) {}

		inline RecordT* getBuffer() const {
			return data;
		}

		inline size_t getBufferSize() const {
			return dataSize;
		}

		virtual size_t tell() const {
			return writeIndex;
		}

		virtual void seek(off_t offset, Stream::SeekWhence whence) {
			off_t target;
			switch(whence) {
				case Stream::OFFSET_FROM_START:
					target = offset;
					break;
				case Stream::OFFSET_FROM_HERE:
					target = static_cast<off_t>(writeIndex) + offset;
					break;
				case Stream::OFFSET_FROM_END:
					target = static_cast<off_t>(dataSize) + offset;
					break;
				default:
					throw IllegalSeekWhenceError(whence);
			}
			if(target < static_cast<off_t>(0) || static_cast<size_t>(target) > dataSize)
				throw SeekOffsetOutOfBoundsError(target);
			writeIndex = static_cast<size_t>(target);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_MEMORYOUTPUTSTREAM_HPP */
