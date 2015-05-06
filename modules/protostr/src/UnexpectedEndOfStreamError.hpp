#ifndef REDSTRAIN_MOD_PROTOSTR_UNEXPECTEDENDOFSTREAMERROR_HPP
#define REDSTRAIN_MOD_PROTOSTR_UNEXPECTEDENDOFSTREAMERROR_HPP

#include "ProtocolError.hpp"

namespace redengine {
namespace protostr {

	class REDSTRAIN_PROTOSTR_API UnexpectedEndOfStreamError : public ProtocolError {

	  public:
		UnexpectedEndOfStreamError();
		UnexpectedEndOfStreamError(const UnexpectedEndOfStreamError&);

		REDSTRAIN_DECLARE_ERROR(UnexpectedEndOfStreamError)

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_UNEXPECTEDENDOFSTREAMERROR_HPP */
