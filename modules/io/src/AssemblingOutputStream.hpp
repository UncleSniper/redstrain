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
		static const util::MemorySize DEFAULT_SEGMENT_SIZE
				= static_cast<util::MemorySize>(REDSTRAIN_IO_ASSEMBLING_OUTPUT_STREAM_DEFAULT_SEGMENT_SIZE);

	  private:
		struct Segment {

			util::MemorySize size, fill;

			Segment(util::MemorySize, util::MemorySize = static_cast<util::MemorySize>(0u));

			static void* operator new(size_t, size_t);

		};

		typedef std::list<Segment*> Segments;
		typedef Segments::const_iterator SegmentIterator;

		class REDSTRAIN_IO_API DeleteSegments {

		  private:
			Segments* segments;

		  public:
			DeleteSegments(Segments*);
			~DeleteSegments();

			void release();

		};

	  private:
		util::MemorySize segmentSize, dataSize;
		Segments segments;

	  private:
		char* reserveBlock(util::MemorySize);

	  protected:
		virtual void writeBlock(const char*, util::MemorySize);

	  public:
		AssemblingOutputStream(util::MemorySize = DEFAULT_SEGMENT_SIZE);
		AssemblingOutputStream(const AssemblingOutputStream&);
		virtual ~AssemblingOutputStream();

		inline util::MemorySize getSegmentSize() const {
			return segmentSize;
		}

		inline util::MemorySize getDataSize() const {
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
