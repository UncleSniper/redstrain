#include "ProtocolWriter.hpp"
#include "ProtocolStreamError.hpp"
#include "ProtocolOutputStream.hpp"
#include "AddressSpaceExceededError.hpp"
#include "AnnouncedSizeMismatchError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace protostr {

	ProtocolOutputStream::ProtocolOutputStream(ProtocolWriter& writer) : writer(writer) {}

	ProtocolOutputStream::ProtocolOutputStream(const ProtocolOutputStream& stream)
			: Stream(stream), OutputStream(stream), writer(stream.writer) {}

	void ProtocolOutputStream::writeBlock(const char* buffer, MemorySize bufferSize) {
		try {
			writer.writeChunk(buffer, bufferSize);
		}
		catch(const AnnouncedSizeMismatchError& asme) {
			throw ProtocolStreamError(asme.cloneAs<ProtocolError>());
		}
		catch(const AddressSpaceExceededError& asee) {
			throw ProtocolStreamError(asee.cloneAs<ProtocolError>());
		}
	}

}}
