#ifndef REDSTRAIN_MOD_DAMNATION_ORIGINSTACKUNDERFLOWERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_ORIGINSTACKUNDERFLOWERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "DamnError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API OriginStackUnderflowError : public DamnError, public error::StateError {

	  public:
		OriginStackUnderflowError();
		OriginStackUnderflowError(const OriginStackUnderflowError&);

		REDSTRAIN_DECLARE_ERROR(OriginStackUnderflowError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_ORIGINSTACKUNDERFLOWERROR_HPP */
