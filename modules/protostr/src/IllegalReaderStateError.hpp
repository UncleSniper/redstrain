#ifndef REDSTRAIN_MOD_PROTOSTR_ILLEGALREADERSTATEERROR_HPP
#define REDSTRAIN_MOD_PROTOSTR_ILLEGALREADERSTATEERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "ProtocolError.hpp"

namespace redengine {
namespace protostr {

	class REDSTRAIN_PROTOSTR_API IllegalReaderStateError : public error::StateError, public ProtocolError {

	  public:
		IllegalReaderStateError();
		IllegalReaderStateError(const IllegalReaderStateError&);

		REDSTRAIN_DECLARE_ERROR(IllegalReaderStateError)

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_ILLEGALREADERSTATEERROR_HPP */
