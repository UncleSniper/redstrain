#include <redstrain/error/Error.hpp>

#include "Stream.hpp"
#include "StreamCloser.hpp"

using redengine::error::Error;

namespace redengine {
namespace io {

	StreamCloser::StreamCloser(Stream* stream) : stream(stream) {}

	StreamCloser::StreamCloser(Stream& stream) : stream(&stream) {}

	StreamCloser::StreamCloser(const StreamCloser& closer) : stream(closer.stream) {}

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
		catch(...) {}
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

	StreamCloser& StreamCloser::operator=(Stream* stream) {
		this->stream = stream;
		return *this;
	}

	StreamCloser& StreamCloser::operator=(Stream& stream) {
		this->stream = &stream;
		return *this;
	}

	StreamCloser& StreamCloser::operator=(const StreamCloser& closer) {
		close();
		this->stream = closer.stream;
		return *this;
	}

}}
