#include <redstrain/error/UnsupportedOperationError.hpp>

#include "Stream.hpp"

using redengine::error::UnsupportedOperationError;

namespace redengine {
namespace io {

	Stream::Stream(const Stream&) {}

	Stream::Stream() {}

	Stream::~Stream() {}

	void Stream::close() {}

	void Stream::seek(off_t, SeekWhence) {
		throw UnsupportedOperationError("redengine::io::Stream::seek(off_t, SeekWhence)");
	}

	size_t Stream::tell() const {
		throw UnsupportedOperationError("redengine::io::Stream::tell() const");
	}

}}
