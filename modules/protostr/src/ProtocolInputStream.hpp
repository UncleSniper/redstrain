#ifndef REDSTRAIN_MOD_PROTOSTR_PROTOCOLINPUTSTREAM_HPP
#define REDSTRAIN_MOD_PROTOSTR_PROTOCOLINPUTSTREAM_HPP

#include <redstrain/io/InputStream.hpp>

#include "api.hpp"

namespace redengine {
namespace protostr {

	class ProtocolReader;

	class REDSTRAIN_PROTOSTR_API ProtocolInputStream : public io::InputStream<char> {

	  private:
		ProtocolReader& reader;

	  protected:
		virtual size_t readBlock(char*, size_t);

	  public:
		ProtocolInputStream(ProtocolReader&);
		ProtocolInputStream(const ProtocolInputStream&);

		inline ProtocolReader& getProtocolReader() {
			return reader;
		}

		inline const ProtocolReader& getProtocolReader() const {
			return reader;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_PROTOCOLINPUTSTREAM_HPP */
