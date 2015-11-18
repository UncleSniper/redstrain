#include "agnostic.hpp"

using redengine::io::OutputStream;

namespace redengine {
namespace text {

	// ======== StringSink16 ========

	StringSink16::StringSink16() {}

	StringSink16::StringSink16(const String16& value) : value(value) {}

	StringSink16::StringSink16(const StringSink16& sink) : value(sink.value) {}

	// ======== StreamSink16 ========

	StreamSink16::StreamSink16(OutputStream<Char16>& stream) : stream(stream) {}

	StreamSink16::StreamSink16(const StreamSink16& sink) : stream(sink.stream) {}

}}
