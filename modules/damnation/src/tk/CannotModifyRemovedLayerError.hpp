#ifndef REDSTRAIN_MOD_DAMNATION_TK_CANNOTMODIFYREMOVEDLAYERERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_CANNOTMODIFYREMOVEDLAYERERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API CannotModifyRemovedLayerError : public DamnError, public error::StateError {

	  public:
		CannotModifyRemovedLayerError();
		CannotModifyRemovedLayerError(const CannotModifyRemovedLayerError&);

		REDSTRAIN_DECLARE_ERROR(CannotModifyRemovedLayerError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_CANNOTMODIFYREMOVEDLAYERERROR_HPP */
