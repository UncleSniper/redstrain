#ifndef REDSTRAIN_MOD_PROTOSTR_ANNOUNCEDSIZEMISMATCHERROR_HPP
#define REDSTRAIN_MOD_PROTOSTR_ANNOUNCEDSIZEMISMATCHERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "ProtocolError.hpp"

namespace redengine {
namespace protostr {

	class REDSTRAIN_PROTOSTR_API AnnouncedSizeMismatchError : public error::StateError, ProtocolError {

	  public:
		AnnouncedSizeMismatchError();
		AnnouncedSizeMismatchError(const AnnouncedSizeMismatchError&);

		REDSTRAIN_DECLARE_ERROR(AnnouncedSizeMismatchError)

	};

}}

#endif /* REDSTRAIN_MOD_PROTOSTR_ANNOUNCEDSIZEMISMATCHERROR_HPP */
