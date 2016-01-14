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

		virtual util::MemorySize readBlock(RecordT* buffer, util::MemorySize size) {
			util::MemorySize count = input.read(buffer, size);
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

		virtual void seek(util::FileOffset offset, Stream::SeekWhence whence) {
			input.seek(offset, whence);
			this->atEnd = false;
		}

		virtual util::FileSize tell() const {
			return input.tell();
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_PROXYINPUTSTREAM_HPP */
