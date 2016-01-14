#ifndef REDSTRAIN_MOD_IO_PROXYOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_PROXYOUTPUTSTREAM_HPP

#include "OutputStream.hpp"

namespace redengine {
namespace io {

	template<typename RecordT>
	class ProxyOutputStream : public OutputStream<RecordT> {

	  protected:
		OutputStream<RecordT>& output;

	  protected:
		ProxyOutputStream(const ProxyOutputStream& stream)
				: Stream(stream), OutputStream<RecordT>(stream), output(stream.output) {}

		virtual void writeBlock(const RecordT* buffer, util::MemorySize size) {
			output.write(buffer, size);
		}

	  public:
		ProxyOutputStream(OutputStream<RecordT>& output) : output(output) {}

		OutputStream<RecordT>& getBackingOutputStream() {
			return output;
		}

		const OutputStream<RecordT>& getBackingOutputStream() const {
			return output;
		}

		virtual void close() {
			output.close();
		}

		virtual void seek(util::FileOffset offset, Stream::SeekWhence whence) {
			output.seek(offset, whence);
		}

		virtual util::FileSize tell() const {
			return output.tell();
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_PROXYOUTPUTSTREAM_HPP */
