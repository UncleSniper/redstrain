#ifndef REDSTRAIN_MOD_DAMNATION_DAMNERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_DAMNERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API DamnError : public virtual error::Error {

	  public:
		DamnError();
		DamnError(const DamnError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(DamnError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_DAMNERROR_HPP */
