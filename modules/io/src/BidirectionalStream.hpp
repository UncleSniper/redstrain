#ifndef REDSTRAIN_MOD_IO_BIDIRECTIONALSTREAM_HPP
#define REDSTRAIN_MOD_IO_BIDIRECTIONALSTREAM_HPP

#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class BidirectionalStream : public virtual InputStream<RecordT>, public virtual OutputStream<RecordT> {

	  protected:
		BidirectionalStream(const BidirectionalStream& stream)
				: Stream(stream), InputStream<RecordT>(stream), OutputStream<RecordT>(stream) {}

	  public:
		BidirectionalStream() {}

	};

}}

#endif /* REDSTRAIN_MOD_IO_BIDIRECTIONALSTREAM_HPP */
