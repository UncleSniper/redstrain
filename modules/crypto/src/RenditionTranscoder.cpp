#include "RenditionTranscoder.hpp"

using redengine::io::OutputStream;

namespace redengine {
namespace crypto {

	RenditionTranscoder::RenditionTranscoder(OutputStream<char>& sink) : sink(sink) {}

	RenditionTranscoder::RenditionTranscoder(const RenditionTranscoder& transcoder)
			: Stream(transcoder), OutputStream<char>(transcoder), sink(transcoder.sink) {}

	void RenditionTranscoder::flush() {}

	void RenditionTranscoder::close() {
		flush();
		sink.close();
	}

}}
