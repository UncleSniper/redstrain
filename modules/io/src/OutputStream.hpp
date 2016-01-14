#ifndef REDSTRAIN_MOD_IO_OUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_OUTPUTSTREAM_HPP

#include "Stream.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class OutputStream : public virtual Stream {

	  public:
		typedef RecordT Record;

	  protected:
		OutputStream(const OutputStream& stream) : Stream(stream) {}

		virtual void writeBlock(const RecordT*, util::MemorySize) = 0;

	  public:
		OutputStream() {}

		virtual void write(const RecordT* buffer, util::MemorySize size) {
			writeBlock(buffer, size);
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_OUTPUTSTREAM_HPP */
