#ifndef REDSTRAIN_MOD_PROTOSTR_PROTOCOLERROR_HPP
#define REDSTRAIN_MOD_PROTOSTR_PROTOCOLERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace protostr {

	class REDSTRAIN_PROTOSTR_API ProtocolError : public virtual error::Error {

	  public:
		ProtocolError();
		ProtocolError(const ProtocolError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(ProtocolError)

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_PROTOCOLERROR_HPP */
