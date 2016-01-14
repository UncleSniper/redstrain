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
		util::MemorySize dataSize, writeIndex;

	  protected:
		virtual void writeBlock(const RecordT* buffer, util::MemorySize count) {
			util::MemorySize restSize = dataSize - writeIndex;
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
		MemoryOutputStream(RecordT* data, util::MemorySize dataSize) : data(data), dataSize(dataSize),
				writeIndex(static_cast<util::MemorySize>(0u)) {}
		MemoryOutputStream(const MemoryOutputStream& stream) : Stream(stream), OutputStream<RecordT>(stream),
				data(stream.data), dataSize(stream.dataSize), writeIndex(stream.writeIndex) {}

		inline RecordT* getBuffer() const {
			return data;
		}

		inline util::MemorySize getBufferSize() const {
			return dataSize;
		}

		virtual util::FileSize tell() const {
			return static_cast<util::FileSize>(writeIndex);
		}

		virtual void seek(util::FileOffset offset, Stream::SeekWhence whence) {
			util::FileOffset target;
			switch(whence) {
				case Stream::OFFSET_FROM_START:
					target = offset;
					break;
				case Stream::OFFSET_FROM_HERE:
					target = static_cast<util::FileOffset>(writeIndex) + offset;
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
			writeIndex = static_cast<util::MemorySize>(target);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_MEMORYOUTPUTSTREAM_HPP */
