#ifndef REDSTRAIN_MOD_DAMNATION_TK_TABINDEXOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_TABINDEXOUTOFBOUNDSERROR_HPP

#include <redstrain/error/IndexOutOfBoundsError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API TabIndexOutOfBoundsError : public DamnError, public error::IndexOutOfBoundsError {

	  public:
		TabIndexOutOfBoundsError(util::MemorySize);
		TabIndexOutOfBoundsError(const TabIndexOutOfBoundsError&);

		REDSTRAIN_DECLARE_ERROR(TabIndexOutOfBoundsError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_TABINDEXOUTOFBOUNDSERROR_HPP */
