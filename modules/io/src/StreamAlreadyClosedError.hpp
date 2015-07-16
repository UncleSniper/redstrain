#ifndef REDSTRAIN_MOD_IO_STREAMALREADYCLOSED_HPP
#define REDSTRAIN_MOD_IO_STREAMALREADYCLOSED_HPP

#include <redstrain/error/IOError.hpp>
#include <redstrain/error/StateError.hpp>

#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API StreamAlreadyClosedError : public error::IOError, public error::StateError {

	  public:
		StreamAlreadyClosedError();
		StreamAlreadyClosedError(const StreamAlreadyClosedError&);

		REDSTRAIN_DECLARE_ERROR(StreamAlreadyClosedError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_STREAMALREADYCLOSED_HPP */
