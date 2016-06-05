#ifndef REDSTRAIN_MOD_ERROR_FRAMEINDEXOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_ERROR_FRAMEINDEXOUTOFBOUNDSERROR_HPP

#include "IndexOutOfBoundsError.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API FrameIndexOutOfBoundsError : public IndexOutOfBoundsError {

	  public:
		FrameIndexOutOfBoundsError(util::MemorySize);
		FrameIndexOutOfBoundsError(const FrameIndexOutOfBoundsError&);

		REDSTRAIN_DECLARE_ERROR(FrameIndexOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_FRAMEINDEXOUTOFBOUNDSERROR_HPP */
