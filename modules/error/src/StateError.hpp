#ifndef REDSTRAIN_MOD_ERROR_STATEERROR_HPP
#define REDSTRAIN_MOD_ERROR_STATEERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StateError : public virtual Error {

	  public:
		StateError();
		StateError(const StateError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(StateError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STATEERROR_HPP */
