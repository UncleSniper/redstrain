#include <redstrain/error/UnsupportedOperationError.hpp>

#include "Stream.hpp"

using redengine::util::FileSize;
using redengine::util::FileOffset;
using redengine::error::UnsupportedOperationError;

namespace redengine {
namespace io {

	Stream::Stream(const Stream&) {}

	Stream::Stream() {}

	Stream::~Stream() {}

	void Stream::close() {}

	void Stream::seek(FileOffset, SeekWhence) {
		throw UnsupportedOperationError("redengine::io::Stream::seek(FileOffset, SeekWhence)");
	}

	FileSize Stream::tell() const {
		throw UnsupportedOperationError("redengine::io::Stream::tell() const");
	}

}}
