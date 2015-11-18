#ifndef REDSTRAIN_MOD_IO_ILLEGALSEEKWHENCEERROR_HPP
#define REDSTRAIN_MOD_IO_ILLEGALSEEKWHENCEERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "Stream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API IllegalSeekWhenceError : public error::IllegalArgumentError {

	  private:
		const Stream::SeekWhence whence;

	  public:
		IllegalSeekWhenceError(Stream::SeekWhence);
		IllegalSeekWhenceError(const IllegalSeekWhenceError&);

		inline Stream::SeekWhence getSeekWhence() const {
			return whence;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalSeekWhenceError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_ILLEGALSEEKWHENCEERROR_HPP */
