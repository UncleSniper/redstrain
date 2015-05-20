#ifndef REDSTRAIN_MOD_IO_STREAMBUFFERFULLERROR_HPP
#define REDSTRAIN_MOD_IO_STREAMBUFFERFULLERROR_HPP

#include <redstrain/error/IOError.hpp>
#include <redstrain/error/StateError.hpp>

#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API StreamBufferFullError : public error::IOError, public error::StateError {

	  public:
		StreamBufferFullError();
		StreamBufferFullError(const StreamBufferFullError&);

		REDSTRAIN_DECLARE_ERROR(StreamBufferFullError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_STREAMBUFFERFULLERROR_HPP */
