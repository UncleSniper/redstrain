#ifndef REDSTRAIN_MOD_IO_STRINGOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_STRINGOUTPUTSTREAM_HPP

#include <string>

#include "OutputStream.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class StringOutputStream : public virtual OutputStream<RecordT> {

	  public:
		typedef typename std::basic_string<RecordT> String;

	  private:
		String& sink;

	  protected:
		virtual void writeBlock(const RecordT* data, size_t dataSize) {
			sink.append(data, static_cast<typename String::size_type>(dataSize));
		}

	  public:
		StringOutputStream(String& sink) : sink(sink) {}
		StringOutputStream(const StringOutputStream& stream)
				: Stream(stream), OutputStream<RecordT>(stream), sink(stream.sink) {}

	};

}}

#endif /* REDSTRAIN_MOD_IO_STRINGOUTPUTSTREAM_HPP */
