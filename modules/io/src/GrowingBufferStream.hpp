#ifndef REDSTRAIN_MOD_IO_GROWINGBUFFERSTREAM_HPP
#define REDSTRAIN_MOD_IO_GROWINGBUFFERSTREAM_HPP

#include <list>

#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class GrowingBufferStream : public InputStream<RecordT>, public OutputStream<RecordT> {

	  private:
		typedef typename std::list<RecordT*> Segments;
		typedef typename Segments::const_iterator SegmentIterator;

		class DeleteSegments {

		  private:
			Segments* segments;
			size_t segmentSize, readIndex, writeIndex;

		  public:
			DeleteSegments(Segments* segments, size_t segmentSize, size_t readIndex, size_t writeIndex)
					: segments(segments), segmentSize(segmentSize), readIndex(readIndex), writeIndex(writeIndex) {}

			~DeleteSegments() {
				if(!segments)
					return;
				SegmentIterator begin(segments->begin()), end(segments->end());
				for(; begin != end; ++begin) {
					size_t killEnd = writeIndex > segmentSize ? segmentSize : writeIndex;
					RecordT *kbegin = *begin + readIndex, *kend = *begin + killEnd;
					for(; kbegin != kend; ++kbegin)
						kbegin->~RecordT();
					delete[] static_cast<char*>(*begin);
					readIndex = static_cast<size_t>(0u);
					writeIndex -= killEnd;
				}
			}

			void release() {
				segments = NULL;
			}

		};

		class DeleteSegment {

		  private:
			RecordT* segment;
			size_t offset;

		  public:
			size_t count;

		  public:
			DeleteSegment(RecordT* segment, size_t offset)
					: segment(segment), offset(offset), count(static_cast<size_t>(0u)) {}

			~DeleteSegment() {
				if(!segment)
					return;
				RecordT* begin = segment + offset;
				RecordT* end = begin + count;
				for(; begin != end; ++begin)
					begin->~RecordT();
				delete[] static_cast<char*>(segment);
			}

			RecordT* release() {
				RecordT* ptr = segment;
				segment = NULL;
				return ptr;
			}

			RecordT* operator*() const {
				return segment;
			}

		};

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
		size_t segmentSize, readIndex, writeIndex;
		Segments segments;

	  protected:
		virtual size_t readBlock(RecordT* buffer, size_t bufferSize) {
			size_t restSize = getDataSize();
			if(bufferSize < restSize)
				restSize = bufferSize;
			if(!restSize)
				return static_cast<size_t>(0u);
			DestroyRecords destroy(buffer);
			size_t offset;
			const RecordT *src, *end;
			RecordT *dest = buffer;
			if(readIndex) {
				offset = segmentSize - readIndex;
				if(restSize < offset)
					offset = restSize;
				src = segments.front() + readIndex;
				end = src + offset;
				for(; src != end; ++src, ++dest) {
					new(dest) RecordT(*src);
					src->~RecordT();
					++readIndex;
					++destroy.count;
				}
				// reading this segment read the entire stream
				if(readIndex == writeIndex) {
					delete[] static_cast<char*>(segments.front());
					segments.pop_front();
					readIndex = writeIndex = static_cast<size_t>(0u);
					return offset;
				}
				// read the rest of this segment, more segments might follow
				if(readIndex == segmentSize) {
					delete[] static_cast<char*>(segments.front());
					segments.pop_front();
					readIndex = static_cast<size_t>(0u);
					writeIndex -= segmentSize;
				}
				// read only part of this segment, but there is still data in the stream
				// => buffer was too small
				else
					return offset;
			}
			else
				offset = static_cast<size_t>(0u);
			while(offset < restSize) {
				size_t chunkSize = offset + segmentSize > restSize ? restSize - offset : segmentSize;
				src = segments.front();
				end = src + chunkSize;
				for(; src != end; ++src, ++dest) {
					new(dest) RecordT(*src);
					src->~RecordT();
					++readIndex;
					++destroy.count;
				}
				offset += chunkSize;
				if(chunkSize == segmentSize) {
					delete[] static_cast<char*>(segments.front());
					segments.pop_front();
					readIndex = static_cast<size_t>(0u);
					writeIndex -= segmentSize;
				}
			}
			destroy.release();
			return restSize;
		}

		virtual void writeBlock(const RecordT* buffer, size_t count) {
			if(!count)
				return;
			size_t segmentOffset = writeIndex % segmentSize, offset;
			const RecordT *src = buffer, *end;
			RecordT* dest;
			if(segmentOffset) {
				offset = segmentSize - segmentOffset;
				if(count < offset)
					offset = count;
				dest = segments.back() + segmentOffset;
				end = src + offset;
				for(; src != end; ++src, ++dest) {
					new(dest) RecordT(*src);
					++writeIndex;
				}
			}
			else
				offset = static_cast<size_t>(0u);
			while(offset < count) {
				size_t chunkSize = count - offset;
				if(segmentSize < chunkSize)
					chunkSize = segmentSize;
				DeleteSegment segment(static_cast<RecordT*>(new char[segmentSize * sizeof(RecordT)]),
						static_cast<size_t>(0u));
				dest = *segment;
				end = src + chunkSize;
				for(; src != end; ++src, ++dest) {
					new(dest) RecordT(*src);
					++segment.count;
				}
				segments.push_back(*segment);
				segment.release();
				writeIndex += chunkSize;
				offset += chunkSize;
			}
		}

	  public:
		GrowingBufferStream(size_t segmentSize =
				static_cast<size_t>(REDSTRAIN_IO_GROWING_BUFFER_STREAM_DEFAULT_SEGMENT_SIZE))
				: segmentSize(segmentSize ? segmentSize
				: static_cast<size_t>(REDSTRAIN_IO_GROWING_BUFFER_STREAM_DEFAULT_SEGMENT_SIZE)),
				readIndex(static_cast<size_t>(0u)), writeIndex(static_cast<size_t>(0u)) {}

		GrowingBufferStream(const GrowingBufferStream& stream)
				: Stream(stream), InputStream<RecordT>(stream), OutputStream<RecordT>(stream),
				segmentSize(stream.segmentSize), readIndex(stream.readIndex), writeIndex(stream.writeIndex) {
			DeleteSegments rollback(&segments, segmentSize, readIndex, writeIndex);
			SegmentIterator begin(stream.segments.begin()), end(stream.segments.end());
			size_t srcRead = readIndex, srcWrite = writeIndex;
			for(; begin != end; ++begin) {
				size_t copyEnd = srcWrite > segmentSize ? segmentSize : srcWrite;
				const RecordT *srcBegin = *begin + srcRead, *srcEnd = *begin + copyEnd;
				DeleteSegment segment(static_cast<RecordT*>(new char[segmentSize * sizeof(RecordT)]), srcRead);
				RecordT* dest = *segment + srcRead;
				for(; srcBegin != srcEnd; ++srcBegin, ++dest) {
					new(dest) RecordT(*srcBegin);
					++segment.count;
				}
				segments.push_back(*segment);
				segment.release();
				srcRead = static_cast<size_t>(0u);
				srcWrite -= copyEnd;
			}
			rollback.release();
		}

		virtual ~GrowingBufferStream() {
			SegmentIterator begin(segments.begin()), end(segments.end());
			for(; begin != end; ++begin) {
				size_t killEnd = writeIndex > segmentSize ? segmentSize : writeIndex;
				RecordT *kbegin = *begin + readIndex, *kend = *begin + killEnd;
				for(; kbegin != kend; ++kbegin)
					kbegin->~RecordT();
				delete[] static_cast<char*>(*begin);
				readIndex = static_cast<size_t>(0u);
				writeIndex -= killEnd;
			}
		}

		inline size_t getSegmentSize() const {
			return segmentSize;
		}

		inline size_t getDataSize() const {
			return writeIndex - readIndex;
		}

		RecordT* getData() const {
			size_t size = writeIndex - readIndex;
			if(!size)
				return NULL;
			DeleteSegment buffer(static_cast<RecordT*>(new char[segmentSize * sizeof(RecordT)]),
					static_cast<size_t>(0u));
			RecordT* dest = *buffer;
			SegmentIterator begin(segments.begin()), end(segments.end());
			size_t srcRead = readIndex, srcWrite = writeIndex;
			for(; begin != end; ++begin) {
				size_t copyEnd = srcWrite > segmentSize ? segmentSize : srcWrite;
				const RecordT *srcBegin = *begin + srcRead, *srcEnd = *begin + copyEnd;
				for(; srcBegin != srcEnd; ++srcBegin, ++dest) {
					new(dest) RecordT(*srcBegin);
					++buffer.count;
				}
				srcRead = static_cast<size_t>(0u);
				srcWrite -= copyEnd;
			}
			return buffer.release();
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_GROWINGBUFFERSTREAM_HPP */
