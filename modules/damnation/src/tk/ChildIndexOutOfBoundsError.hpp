#ifndef REDSTRAIN_MOD_DAMNATION_TK_CHILDINDEXOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_CHILDINDEXOUTOFBOUNDSERROR_HPP

#include <redstrain/error/IndexOutOfBoundsError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API ChildIndexOutOfBoundsError : public DamnError, public error::IndexOutOfBoundsError {

	  public:
		ChildIndexOutOfBoundsError(util::MemorySize);
		ChildIndexOutOfBoundsError(const ChildIndexOutOfBoundsError&);

		REDSTRAIN_DECLARE_ERROR(ChildIndexOutOfBoundsError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_CHILDINDEXOUTOFBOUNDSERROR_HPP */
