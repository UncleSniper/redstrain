#ifndef REDSTRAIN_MOD_UTIL_STRINGBUILDER_HPP
#define REDSTRAIN_MOD_UTIL_STRINGBUILDER_HPP

#include <string>
#include <cstddef>
#include <cstring>

#include "types.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace util {

	template<
		typename CharT,
		MemorySize SegmentSize = static_cast<MemorySize>(REDSTRAIN_UTIL_STRINGBUILDER_DEFAULT_SEGMENT_SIZE)
	>
	class StringBuilder {

	  public:
		typedef CharT Char;

	  public:
		static const MemorySize SEGMENT_SIZE = SegmentSize;

	  private:
		struct Segment {
			MemorySize fill;
			CharT data[SegmentSize];
			Segment* next;
		};

		struct DeleteSegments {

			Segment *const* segments;

			DeleteSegments(Segment *const* segments) : segments(segments) {}

			~DeleteSegments() {
				if(!segments)
					return;
				Segment *current = *segments, *next;
				while(current) {
					next = current->next;
					delete current;
					current = next;
				}
			}

		};

	  private:
		MemorySize length;
		Segment *first, *last;

	  public:
		StringBuilder() : length(static_cast<MemorySize>(0u)), first(NULL), last(NULL) {}

		StringBuilder(const StringBuilder& builder) : length(static_cast<MemorySize>(0u)), first(NULL), last(NULL) {
			DeleteSegments delSegs(&first);
			const Segment* segment;
			for(segment = builder.first; segment; segment = segment->next)
				append(segment->data, segment->fill);
			delSegs.segments = NULL;
		}

		~StringBuilder() {
			Segment *current = first, *next;
			while(current) {
				next = current->next;
				delete current;
				current = next;
			}
		}

		inline MemorySize getLength() const {
			return length;
		}

		void append(const CharT* buffer, MemorySize size) {
			MemorySize chunkSize;
			while(size) {
				if(!last || last->fill == SegmentSize) {
					Segment* segment = new Segment;
					segment->fill = static_cast<MemorySize>(0u);
					segment->next = NULL;
					chunkSize = SegmentSize;
					if(last)
						last->next = segment;
					else
						first = segment;
					last = segment;
				}
				else
					chunkSize = SegmentSize - last->fill;
				if(chunkSize > size)
					chunkSize = size;
				memcpy(last->data + last->fill, buffer, static_cast<size_t>(chunkSize) * sizeof(CharT));
				last->fill += chunkSize;
				length += chunkSize;
				buffer += chunkSize;
				size -= chunkSize;
			}
		}

		void append(const std::basic_string<CharT>& string) {
			append(string.data(), static_cast<MemorySize>(string.length()));
		}

		void append(CharT c) {
			if(!last || last->fill == SegmentSize) {
				Segment* segment = new Segment;
				segment->fill = static_cast<MemorySize>(1u);
				segment->next = NULL;
				if(last)
					last->next = segment;
				else
					first = segment;
				last = segment;
				*segment->data = c;
			}
			else {
				last->data[last->fill] = c;
				++last->fill;
			}
			++length;
		}

		void copyTo(CharT* buffer) {
			const Segment* segment;
			for(segment = first; segment; segment = segment->next) {
				memcpy(buffer, segment->data, static_cast<size_t>(segment->fill) * sizeof(CharT));
				buffer += segment->fill;
			}
		}

		void appendTo(std::basic_string<CharT>& string) {
			const Segment* segment;
			for(segment = first; segment; segment = segment->next)
				string.append(segment->data,
						static_cast<typename std::basic_string<CharT>::size_type>(segment->fill));
		}

		std::basic_string<CharT> toString() {
			std::basic_string<CharT> result;
			result.reserve(static_cast<typename std::basic_string<CharT>::size_type>(length));
			appendTo(result);
			return result;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_STRINGBUILDER_HPP */
