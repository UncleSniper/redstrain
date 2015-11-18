#ifndef REDSTRAIN_MOD_ERROR_ILLEGALARGUMENTERROR_HPP
#define REDSTRAIN_MOD_ERROR_ILLEGALARGUMENTERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IllegalArgumentError : public virtual Error {

	  public:
		IllegalArgumentError();
		IllegalArgumentError(const IllegalArgumentError&);

		REDSTRAIN_DECLARE_ERROR(IllegalArgumentError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ILLEGALARGUMENTERROR_HPP */
