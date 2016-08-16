#ifndef REDSTRAIN_MOD_ERROR_UNCAUGHTEXCEPTIONERROR_HPP
#define REDSTRAIN_MOD_ERROR_UNCAUGHTEXCEPTIONERROR_HPP

#include "Error.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API UncaughtExceptionError : public Error {

	  public:
		UncaughtExceptionError();
		UncaughtExceptionError(const UncaughtExceptionError&);

		REDSTRAIN_DECLARE_ERROR(UncaughtExceptionError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_UNCAUGHTEXCEPTIONERROR_HPP */
