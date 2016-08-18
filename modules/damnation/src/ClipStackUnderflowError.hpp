#ifndef REDSTRAIN_MOD_DAMNATION_CLIPSTACKUNDERFLOWERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_CLIPSTACKUNDERFLOWERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "DamnError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API ClipStackUnderflowError : public DamnError, public error::StateError {

	  public:
		ClipStackUnderflowError();
		ClipStackUnderflowError(const ClipStackUnderflowError&);

		REDSTRAIN_DECLARE_ERROR(ClipStackUnderflowError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_CLIPSTACKUNDERFLOWERROR_HPP */
