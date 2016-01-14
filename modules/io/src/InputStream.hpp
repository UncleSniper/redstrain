#ifndef REDSTRAIN_MOD_IO_INPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_INPUTSTREAM_HPP

#include "OutputStream.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class InputStream : public virtual Stream {

	  public:
		typedef RecordT Record;

	  public:
		static const util::MemorySize COPY_BUFFER_BYTES
				= static_cast<util::MemorySize>(REDSTRAIN_IO_DEFAULT_COPY_BUFFER_BYTES);
		static const util::MemorySize COPY_BUFFER_RECORDS
				= COPY_BUFFER_BYTES / static_cast<util::MemorySize>(sizeof(RecordT))
				? COPY_BUFFER_BYTES / static_cast<util::MemorySize>(sizeof(RecordT))
				: static_cast<util::MemorySize>(1u);

	  protected:
		bool atEnd;

	  protected:
		InputStream(const InputStream& stream) : Stream(stream) {}

		virtual util::MemorySize readBlock(RecordT*, util::MemorySize) = 0;

	  public:
		InputStream() : atEnd(false) {}

		inline bool isAtEnd() const {
			return atEnd;
		}

		virtual util::MemorySize read(RecordT* buffer, util::MemorySize size) {
			util::MemorySize count = readBlock(buffer, size);
			if(!count)
				atEnd = true;
			return count;
		}

		util::FileSize copyTo(OutputStream<RecordT>& sink) {
			//TODO: make safe for arbitrary RecordT
			RecordT buffer[COPY_BUFFER_RECORDS];
			util::FileSize total = static_cast<util::FileSize>(0u);
			for(;;) {
				util::MemorySize count = readBlock(buffer, COPY_BUFFER_RECORDS);
				if(!count)
					break;
				sink.write(buffer, count);
				total += static_cast<util::FileSize>(count);
			}
			atEnd = true;
			return total;
		}

		util::FileSize copyTo(OutputStream<RecordT>& sink, util::FileSize limit) {
			//TODO: make safe for arbitrary RecordT
			RecordT buffer[COPY_BUFFER_RECORDS];
			util::FileSize remaining = limit;
			while(remaining) {
				util::MemorySize chunkSize = COPY_BUFFER_RECORDS;
				if(remaining < static_cast<util::FileSize>(chunkSize))
					chunkSize = static_cast<util::MemorySize>(remaining);
				util::MemorySize count = readBlock(buffer, chunkSize);
				if(!count) {
					atEnd = true;
					break;
				}
				sink.write(buffer, count);
				remaining -= static_cast<util::FileSize>(count);
			}
			return limit - remaining;
		}

		template<typename CollectionT>
		util::FileSize teeTo(const CollectionT& sinks) {
			//TODO: make safe for arbitrary RecordT
			RecordT buffer[COPY_BUFFER_RECORDS];
			typename CollectionT::const_iterator sinksBegin, sinksEnd;
			util::FileSize total = static_cast<util::FileSize>(0u);
			for(;;) {
				util::MemorySize count = readBlock(buffer, COPY_BUFFER_RECORDS);
				if(!count)
					break;
				sinksBegin = sinks.begin();
				sinksEnd = sinks.end();
				for(; sinksBegin != sinksEnd; ++sinksBegin)
					(*sinksBegin)->write(buffer, count);
				total += static_cast<util::FileSize>(count);
			}
			atEnd = true;
			return total;
		}

		template<typename CollectionT>
		util::FileSize teeTo(const CollectionT& sinks, util::FileSize limit) {
			//TODO: make safe for arbitrary RecordT
			RecordT buffer[COPY_BUFFER_RECORDS];
			typename CollectionT::const_iterator sinksBegin, sinksEnd;
			util::FileSize remaining = limit;
			while(remaining) {
				util::MemorySize chunkSize = sizeof(buffer);
				if(remaining < static_cast<util::FileSize>(chunkSize))
					chunkSize = static_cast<util::MemorySize>(remaining);
				util::MemorySize count = readBlock(buffer, chunkSize);
				if(!count) {
					atEnd = true;
					break;
				}
				sinksBegin = sinks.begin();
				sinksEnd = sinks.end();
				for(; sinksBegin != sinksEnd; ++sinksBegin)
					(*sinksBegin)->write(buffer, count);
				remaining -= static_cast<util::FileSize>(count);
			}
			return limit - remaining;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_INPUTSTREAM_HPP */
