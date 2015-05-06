#ifndef REDSTRAIN_MOD_PROTOSTR_PROTOCOLSTREAMERROR_HPP
#define REDSTRAIN_MOD_PROTOSTR_PROTOCOLSTREAMERROR_HPP

#include <redstrain/util/Ref.hpp>
#include <redstrain/error/IOError.hpp>

#include "api.hpp"

namespace redengine {
namespace protostr {

	class ProtocolError;

	class REDSTRAIN_PROTOSTR_API ProtocolStreamError : public error::IOError {

	  private:
		const util::Ref<ProtocolError> cause;

	  public:
		ProtocolStreamError(ProtocolError*);
		ProtocolStreamError(const ProtocolStreamError&);

		ProtocolError* getCause() const;

		REDSTRAIN_DECLARE_ERROR(ProtocolStreamError)

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_PROTOCOLSTREAMERROR_HPP */
