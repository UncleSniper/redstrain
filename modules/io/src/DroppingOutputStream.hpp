#ifndef REDSTRAIN_MOD_IO_DROPPINGOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_DROPPINGOUTPUTSTREAM_HPP

#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "OutputStream.hpp"
#include "SeekOffsetOutOfBoundsError.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class DroppingOutputStream : public OutputStream<RecordT> {

	  private:
		size_t offset, size;

	  protected:
		virtual void writeBlock(const RecordT*, size_t count) {
			offset += count;
			if(offset > size)
				size = offset;
		}

	  public:
		DroppingOutputStream() : offset(static_cast<size_t>(0u)), size(static_cast<size_t>(0u)) {}
		DroppingOutputStream(const DroppingOutputStream& stream)
				: Stream(stream), OutputStream<RecordT>(stream), offset(stream.offset), size(stream.size) {}

		virtual void seek(off_t targetOffset, Stream::SeekWhence whence) {
			switch(whence) {
				case Stream::OFFSET_FROM_END:
					targetOffset += static_cast<off_t>(size);
				case Stream::OFFSET_FROM_START:
				withTargetOffset:
					if(targetOffset < static_cast<off_t>(0))
						throw SeekOffsetOutOfBoundsError(targetOffset);
					offset = static_cast<size_t>(targetOffset);
					break;
				case Stream::OFFSET_FROM_HERE:
					targetOffset += static_cast<off_t>(offset);
					goto withTargetOffset;
				default:
					throw error::IllegalArgumentError("Not a valid SeekWhence: "
							+ util::StringUtils::toString(static_cast<int>(whence)));
			}
		}

		virtual size_t tell() const {
			return offset;
		}

		inline size_t getDataSize() const {
			return size;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_DROPPINGOUTPUTSTREAM_HPP */
