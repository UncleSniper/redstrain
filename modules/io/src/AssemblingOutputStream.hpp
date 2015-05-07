#ifndef REDSTRAIN_MOD_IO_ASSEMBLINGOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_ASSEMBLINGOUTPUTSTREAM_HPP

#include <list>

#include "Patch.hpp"
#include "OutputStream.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API AssemblingOutputStream : public OutputStream<char> {

	  public:
		static const size_t DEFAULT_SEGMENT_SIZE
				= static_cast<size_t>(REDSTRAIN_IO_ASSEMBLING_OUTPUT_STREAM_DEFAULT_SEGMENT_SIZE);

	  private:
		struct Segment {

			size_t size, fill;

			Segment(size_t, size_t = static_cast<size_t>(0u));

		};

		typedef std::list<Segment*> Segments;
		typedef Segments::const_iterator SegmentIterator;

		class DeleteSegments {

		  private:
			Segments* segments;

		  public:
			DeleteSegments(Segments*);
			~DeleteSegments();

			void release();

		};

	  private:
		size_t segmentSize, dataSize;
		Segments segments;

	  private:
		char* reserveBlock(size_t);

	  protected:
		virtual void writeBlock(const char*, size_t);

	  public:
		AssemblingOutputStream(size_t = DEFAULT_SEGMENT_SIZE);
		AssemblingOutputStream(const AssemblingOutputStream&);
		virtual ~AssemblingOutputStream();

		inline size_t getSegmentSize() const {
			return segmentSize;
		}

		inline size_t getDataSize() const {
			return dataSize;
		}

		char* getData() const;

		Patch<int8_t> reserveInt8();
		Patch<uint8_t> reserveUInt8();
		Patch<int16_t> reserveInt16();
		Patch<uint16_t> reserveUInt16();
		Patch<int32_t> reserveInt32();
		Patch<uint32_t> reserveUInt32();
		Patch<int64_t> reserveInt64();
		Patch<uint64_t> reserveUInt64();
		Patch<float> reserveFloat32();
		Patch<double> reserveFloat64();

	};

}}

#endif /* REDSTRAIN_MOD_IO_ASSEMBLINGOUTPUTSTREAM_HPP */
