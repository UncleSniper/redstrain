#ifndef REDSTRAIN_MOD_ERROR_LISTINDEXOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_ERROR_LISTINDEXOUTOFBOUNDSERROR_HPP

#include "IndexOutOfBoundsError.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API ListIndexOutOfBoundsError : public IndexOutOfBoundsError {

	  public:
		ListIndexOutOfBoundsError(util::MemorySize);
		ListIndexOutOfBoundsError(const ListIndexOutOfBoundsError&);

		REDSTRAIN_DECLARE_ERROR(ListIndexOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_LISTINDEXOUTOFBOUNDSERROR_HPP */
