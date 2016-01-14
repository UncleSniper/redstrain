#ifndef REDSTRAIN_MOD_IO_DROPPINGOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_DROPPINGOUTPUTSTREAM_HPP

#include <redstrain/util/StringUtils.hpp>

#include "OutputStream.hpp"
#include "IllegalSeekWhenceError.hpp"
#include "SeekOffsetOutOfBoundsError.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class DroppingOutputStream : public OutputStream<RecordT> {

	  private:
		util::FileSize offset, size;

	  protected:
		virtual void writeBlock(const RecordT*, util::MemorySize count) {
			offset += static_cast<util::FileSize>(count);
			if(offset > size)
				size = offset;
		}

	  public:
		DroppingOutputStream() : offset(static_cast<util::FileSize>(0u)), size(static_cast<util::FileSize>(0u)) {}

		DroppingOutputStream(const DroppingOutputStream& stream)
				: Stream(stream), OutputStream<RecordT>(stream), offset(stream.offset), size(stream.size) {}

		virtual void seek(util::FileOffset targetOffset, Stream::SeekWhence whence) {
			switch(whence) {
				case Stream::OFFSET_FROM_END:
					targetOffset += static_cast<util::FileOffset>(size);
				case Stream::OFFSET_FROM_START:
				withTargetOffset:
					if(targetOffset < static_cast<util::FileOffset>(0))
						throw SeekOffsetOutOfBoundsError(targetOffset);
					offset = static_cast<util::FileSize>(targetOffset);
					break;
				case Stream::OFFSET_FROM_HERE:
					targetOffset += static_cast<util::FileOffset>(offset);
					goto withTargetOffset;
				default:
					throw IllegalSeekWhenceError(whence);
			}
		}

		virtual util::FileSize tell() const {
			return offset;
		}

		inline util::FileSize getDataSize() const {
			return size;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_DROPPINGOUTPUTSTREAM_HPP */
