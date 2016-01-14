#include <cstring>
#include <redstrain/util/Delete.hpp>

#include "AssemblingOutputStream.hpp"

using redengine::util::Delete;
using redengine::util::MemorySize;

namespace redengine {
namespace io {

	AssemblingOutputStream::Segment::Segment(MemorySize size, MemorySize fill) : size(size), fill(fill) {}

	void* AssemblingOutputStream::Segment::operator new(size_t allocSize, size_t segmentSize) {
		return ::operator new(allocSize + segmentSize);
	}

	AssemblingOutputStream::DeleteSegments::DeleteSegments(Segments* segments) : segments(segments) {}

	AssemblingOutputStream::DeleteSegments::~DeleteSegments() {
		if(segments) {
			SegmentIterator begin(segments->begin()), end(segments->end());
			for(; begin != end; ++begin)
				delete *begin;
		}
	}

	void AssemblingOutputStream::DeleteSegments::release() {
		segments = NULL;
	}

	AssemblingOutputStream::AssemblingOutputStream(MemorySize segmentSize)
			: segmentSize(segmentSize ? segmentSize : DEFAULT_SEGMENT_SIZE),
			dataSize(static_cast<MemorySize>(0u)) {}

	AssemblingOutputStream::AssemblingOutputStream(const AssemblingOutputStream& stream)
			: Stream(stream), OutputStream(stream), segmentSize(stream.segmentSize), dataSize(stream.dataSize) {
		DeleteSegments rollback(&segments);
		SegmentIterator begin(stream.segments.begin()), end(stream.segments.end());
		for(; begin != end; ++begin) {
			Delete<Segment> segment(new((*begin)->size) Segment((*begin)->size, (*begin)->fill));
			memcpy(*segment + 1, *begin + 1, static_cast<size_t>((*begin)->fill));
			segments.push_back(*segment);
			segment.set();
		}
		rollback.release();
	}

	AssemblingOutputStream::~AssemblingOutputStream() {
		SegmentIterator begin(segments.begin()), end(segments.end());
		for(; begin != end; ++begin)
			delete *begin;
	}

	char* AssemblingOutputStream::getData() const {
		if(!dataSize)
			return NULL;
		char* buffer = new char[dataSize];
		SegmentIterator begin(segments.begin()), end(segments.end());
		MemorySize offset = static_cast<MemorySize>(0u);
		for(; begin != end; ++begin) {
			memcpy(buffer + offset, *begin + 1, (*begin)->fill);
			offset += (*begin)->fill;
		}
		return buffer;
	}

	void AssemblingOutputStream::writeBlock(const char* buffer, MemorySize count) {
		if(!count)
			return;
		if(dataSize) {
			Segment* last = segments.back();
			MemorySize rest = last->size - last->fill;
			if(rest >= count) {
				memcpy(reinterpret_cast<char*>(last + 1) + last->fill, buffer, static_cast<size_t>(count));
				last->fill += count;
				dataSize += count;
				return;
			}
		}
		MemorySize size = segmentSize;
		if(count > size)
			size = count;
		Delete<Segment> segment(new(static_cast<size_t>(size)) Segment(size, count));
		memcpy(*segment + 1, buffer, static_cast<size_t>(count));
		segments.push_back(*segment);
		segment.set();
		dataSize += count;
	}

	char* AssemblingOutputStream::reserveBlock(MemorySize size) {
		if(dataSize) {
			Segment* last = segments.back();
			MemorySize rest = last->size - last->fill;
			if(rest >= size) {
				char* block = reinterpret_cast<char*>(last + 1) + last->fill;
				last->fill += size;
				dataSize += size;
				return block;
			}
		}
		MemorySize sz = segmentSize;
		if(size > sz)
			sz = size;
		Delete<Segment> segment(new(static_cast<size_t>(sz)) Segment(sz, size));
		segments.push_back(*segment);
		segment.set();
		dataSize += size;
		return reinterpret_cast<char*>(*segment + 1);
	}

	Patch<int8_t> AssemblingOutputStream::reserveInt8() {
		return Patch<int8_t>(reserveBlock(static_cast<MemorySize>(sizeof(int8_t))));
	}

	Patch<uint8_t> AssemblingOutputStream::reserveUInt8() {
		return Patch<uint8_t>(reserveBlock(static_cast<MemorySize>(sizeof(uint8_t))));
	}

	Patch<int16_t> AssemblingOutputStream::reserveInt16() {
		return Patch<int16_t>(reserveBlock(static_cast<MemorySize>(sizeof(int16_t))));
	}

	Patch<uint16_t> AssemblingOutputStream::reserveUInt16() {
		return Patch<uint16_t>(reserveBlock(static_cast<MemorySize>(sizeof(uint16_t))));
	}

	Patch<int32_t> AssemblingOutputStream::reserveInt32() {
		return Patch<int32_t>(reserveBlock(static_cast<MemorySize>(sizeof(int32_t))));
	}

	Patch<uint32_t> AssemblingOutputStream::reserveUInt32() {
		return Patch<uint32_t>(reserveBlock(static_cast<MemorySize>(sizeof(uint32_t))));
	}

	Patch<int64_t> AssemblingOutputStream::reserveInt64() {
		return Patch<int64_t>(reserveBlock(static_cast<MemorySize>(sizeof(int64_t))));
	}

	Patch<uint64_t> AssemblingOutputStream::reserveUInt64() {
		return Patch<uint64_t>(reserveBlock(static_cast<MemorySize>(sizeof(uint64_t))));
	}

	Patch<float> AssemblingOutputStream::reserveFloat32() {
		return Patch<float>(reserveBlock(static_cast<MemorySize>(sizeof(float))));
	}

	Patch<double> AssemblingOutputStream::reserveFloat64() {
		return Patch<double>(reserveBlock(static_cast<MemorySize>(sizeof(double))));
	}

}}
