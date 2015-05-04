#ifndef REDSTRAIN_MOD_IO_SEEKOFFSETOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_IO_SEEKOFFSETOUTOFBOUNDSERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "Stream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API SeekOffsetOutOfBoundsError : public error::IOError {

	  private:
		const off_t offset;

	  public:
		SeekOffsetOutOfBoundsError(off_t);
		SeekOffsetOutOfBoundsError(const SeekOffsetOutOfBoundsError&);

		inline off_t getTargetOffset() const {
			return offset;
		}

		REDSTRAIN_DECLARE_ERROR(SeekOffsetOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_SEEKOFFSETOUTOFBOUNDSERROR_HPP */
