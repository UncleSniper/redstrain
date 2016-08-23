#ifndef REDSTRAIN_MOD_DAMNATION_TK_CANNOTMODIFYFOREIGNLAYERERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_CANNOTMODIFYFOREIGNLAYERERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API CannotModifyForeignLayerError
			: public DamnError, public error::IllegalArgumentError {

	  public:
		CannotModifyForeignLayerError();
		CannotModifyForeignLayerError(const CannotModifyForeignLayerError&);

		REDSTRAIN_DECLARE_ERROR(CannotModifyForeignLayerError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_CANNOTMODIFYFOREIGNLAYERERROR_HPP */
