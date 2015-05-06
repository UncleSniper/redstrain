#ifndef REDSTRAIN_MOD_PROTOSTR_PROTOCOLOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_PROTOSTR_PROTOCOLOUTPUTSTREAM_HPP

#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"

namespace redengine {
namespace protostr {

	class ProtocolWriter;

	class REDSTRAIN_PROTOSTR_API ProtocolOutputStream : public io::OutputStream<char> {

	  private:
		ProtocolWriter& writer;

	  protected:
		virtual void writeBlock(const char*, size_t);

	  public:
		ProtocolOutputStream(ProtocolWriter&);
		ProtocolOutputStream(const ProtocolOutputStream&);

		inline ProtocolWriter& getProtocolWriter() {
			return writer;
		}

		inline const ProtocolWriter& getProtocolWriter() const {
			return writer;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_PROTOCOLOUTPUTSTREAM_HPP */
