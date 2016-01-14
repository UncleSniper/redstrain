#include "ProtocolReader.hpp"
#include "ProtocolInputStream.hpp"
#include "ProtocolStreamError.hpp"
#include "IllegalReaderStateError.hpp"
#include "UnexpectedEndOfStreamError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace protostr {

	ProtocolInputStream::ProtocolInputStream(ProtocolReader& reader) : reader(reader) {}

	ProtocolInputStream::ProtocolInputStream(const ProtocolInputStream& stream)
			: Stream(stream), InputStream(stream), reader(stream.reader) {}

	MemorySize ProtocolInputStream::readBlock(char* buffer, MemorySize bufferSize) {
		try {
			return reader.readChunk(buffer, bufferSize);
		}
		catch(const IllegalReaderStateError& irse) {
			throw ProtocolStreamError(irse.cloneAs<ProtocolError>());
		}
		catch(const UnexpectedEndOfStreamError& ueose) {
			throw ProtocolStreamError(ueose.cloneAs<ProtocolError>());
		}
	}

}}
