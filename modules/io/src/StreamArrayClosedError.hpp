#ifndef REDSTRAIN_MOD_IO_STREAMARRAYCLOSEDERROR_HPP
#define REDSTRAIN_MOD_IO_STREAMARRAYCLOSEDERROR_HPP

#include <redstrain/error/IOError.hpp>
#include <redstrain/error/StateError.hpp>

#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API StreamArrayClosedError : public error::IOError, public error::StateError {

	  public:
		StreamArrayClosedError();
		StreamArrayClosedError(const StreamArrayClosedError&);

		REDSTRAIN_DECLARE_ERROR(StreamArrayClosedError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_STREAMARRAYCLOSEDERROR_HPP */
