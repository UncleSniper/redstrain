#include <redstrain/error/Error.hpp>

#include "Stream.hpp"
#include "StreamCloser.hpp"

using redengine::error::Error;

namespace redengine {
namespace io {

	StreamCloser::StreamCloser(Stream* stream) : stream(stream) {}

	StreamCloser::StreamCloser(Stream& stream) : stream(&stream) {}

	StreamCloser::~StreamCloser() {
		close();
	}

	void StreamCloser::close() {
		if(!stream)
			return;
		try {
			stream->close();
		}
		catch(const Error&) {}
		stream = NULL;
	}

	void StreamCloser::release() {
		stream = NULL;
	}

	Stream* StreamCloser::set(Stream* newStream) {
		Stream* oldStream = stream;
		stream = newStream;
		return oldStream;
	}

}}
