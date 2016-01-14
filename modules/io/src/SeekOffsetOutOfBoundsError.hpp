#ifndef REDSTRAIN_MOD_IO_SEEKOFFSETOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_IO_SEEKOFFSETOUTOFBOUNDSERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "Stream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API SeekOffsetOutOfBoundsError : public error::IOError {

	  private:
		const util::FileOffset offset;

	  public:
		SeekOffsetOutOfBoundsError(util::FileOffset);
		SeekOffsetOutOfBoundsError(const SeekOffsetOutOfBoundsError&);

		inline util::FileOffset getTargetOffset() const {
			return offset;
		}

		REDSTRAIN_DECLARE_ERROR(SeekOffsetOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_SEEKOFFSETOUTOFBOUNDSERROR_HPP */
