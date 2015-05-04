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
		static const size_t COPY_BUFFER_BYTES = static_cast<size_t>(REDSTRAIN_IO_DEFAULT_COPY_BUFFER_BYTES);
		static const size_t COPY_BUFFER_RECORDS = COPY_BUFFER_BYTES / sizeof(RecordT)
				? COPY_BUFFER_BYTES / sizeof(RecordT) : static_cast<size_t>(1u);

	  protected:
		bool atEnd;

	  protected:
		InputStream(const InputStream& stream) : Stream(stream) {}

		virtual size_t readBlock(RecordT*, size_t) = 0;

	  public:
		InputStream() : atEnd(false) {}

		inline bool isAtEnd() const {
			return atEnd;
		}

		virtual size_t read(RecordT* buffer, size_t size) {
			size_t count = readBlock(buffer, size);
			if(!count)
				atEnd = true;
			return count;
		}

		size_t copyTo(OutputStream<RecordT>& sink) {
			RecordT buffer[COPY_BUFFER_RECORDS];
			size_t total = static_cast<size_t>(0u);
			for(;;) {
				size_t count = readBlock(buffer, sizeof(buffer));
				if(!count)
					break;
				sink.write(buffer, count);
				total += count;
			}
			atEnd = true;
			return total;
		}

		size_t copyTo(OutputStream<RecordT>& sink, size_t limit) {
			RecordT buffer[COPY_BUFFER_RECORDS];
			size_t remaining = limit;
			while(remaining) {
				size_t chunkSize = sizeof(buffer);
				if(remaining < chunkSize)
					chunkSize = remaining;
				size_t count = readBlock(buffer, chunkSize);
				if(!count) {
					atEnd = true;
					break;
				}
				sink.write(buffer, count);
				remaining -= count;
			}
			return limit - remaining;
		}

		template<typename CollectionT>
		size_t teeTo(const CollectionT& sinks) {
			RecordT buffer[COPY_BUFFER_RECORDS];
			typename CollectionT::const_iterator sinksBegin, sinksEnd;
			size_t total = static_cast<size_t>(0u);
			for(;;) {
				size_t count = readBlock(buffer, sizeof(buffer));
				if(!count)
					break;
				sinksBegin = sinks.begin();
				sinksEnd = sinks.end();
				for(; sinksBegin != sinksEnd; ++sinksBegin)
					(*sinksBegin)->write(buffer, count);
				total += count;
			}
			atEnd = true;
			return total;
		}

		template<typename CollectionT>
		size_t teeTo(const CollectionT& sinks, size_t limit) {
			RecordT buffer[COPY_BUFFER_RECORDS];
			typename CollectionT::const_iterator sinksBegin, sinksEnd;
			size_t remaining = limit;
			while(remaining) {
				size_t chunkSize = sizeof(buffer);
				if(remaining < chunkSize)
					chunkSize = remaining;
				size_t count = readBlock(buffer, chunkSize);
				if(!count) {
					atEnd = true;
					break;
				}
				sinksBegin = sinks.begin();
				sinksEnd = sinks.end();
				for(; sinksBegin != sinksEnd; ++sinksBegin)
					(*sinksBegin)->write(buffer, count);
				remaining -= count;
			}
			return limit - remaining;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_INPUTSTREAM_HPP */
