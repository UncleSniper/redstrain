#ifndef REDSTRAIN_MOD_DAMNATION_TK_CANNOTSETPARENTOFWIDGETERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_CANNOTSETPARENTOFWIDGETERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API CannotSetParentOfWidgetError
			: public DamnError, public error::IllegalArgumentError {

	  public:
		CannotSetParentOfWidgetError();
		CannotSetParentOfWidgetError(const CannotSetParentOfWidgetError&);

		REDSTRAIN_DECLARE_ERROR(CannotSetParentOfWidgetError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_CANNOTSETPARENTOFWIDGETERROR_HPP */
