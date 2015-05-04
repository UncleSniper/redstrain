#ifndef REDSTRAIN_MOD_IO_PROXYINPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_PROXYINPUTSTREAM_HPP

#include "InputStream.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class ProxyInputStream : public InputStream<RecordT> {

	  protected:
		InputStream<RecordT>& input;

	  protected:
		ProxyInputStream(const ProxyInputStream& stream)
				: Stream(stream), InputStream<RecordT>(stream), input(stream.input) {}

		virtual size_t readBlock(RecordT* buffer, size_t size) {
			size_t count = input.read(buffer, size);
			if(!count)
				this->atEnd = true;
			return count;
		}

	  public:
		ProxyInputStream(InputStream<RecordT>& input) : input(input) {}

		InputStream<RecordT>& getBackingInputStream() {
			return input;
		}

		const InputStream<RecordT>& getBackingInputStream() const {
			return input;
		}

		virtual void close() {
			input.close();
		}

		virtual void seek(off_t offset, Stream::SeekWhence whence) {
			input.seek(offset, whence);
			this->atEnd = false;
		}

		virtual size_t tell() const {
			return input.tell();
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_PROXYINPUTSTREAM_HPP */
