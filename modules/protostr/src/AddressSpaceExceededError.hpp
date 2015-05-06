#ifndef REDSTRAIN_MOD_PROTOSTR_ADDRESSSPACEEXCEEDEDERROR_HPP
#define REDSTRAIN_MOD_PROTOSTR_ADDRESSSPACEEXCEEDEDERROR_HPP

#include "ProtocolError.hpp"

namespace redengine {
namespace protostr {

	class REDSTRAIN_PROTOSTR_API AddressSpaceExceededError : public ProtocolError {

	  public:
		AddressSpaceExceededError();
		AddressSpaceExceededError(const AddressSpaceExceededError&);

		REDSTRAIN_DECLARE_ERROR(AddressSpaceExceededError)

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_ADDRESSSPACEEXCEEDEDERROR_HPP */
