#ifndef REDSTRAIN_MOD_PROTOSTR_ILLEGALWRITERSTATEERROR_HPP
#define REDSTRAIN_MOD_PROTOSTR_ILLEGALWRITERSTATEERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "ProtocolError.hpp"

namespace redengine {
namespace protostr {

	class REDSTRAIN_PROTOSTR_API IllegalWriterStateError : public error::StateError, public ProtocolError {

	  public:
		IllegalWriterStateError();
		IllegalWriterStateError(const IllegalWriterStateError&);

		REDSTRAIN_DECLARE_ERROR(IllegalWriterStateError)

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_ILLEGALWRITERSTATEERROR_HPP */
