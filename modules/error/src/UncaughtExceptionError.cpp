#include "UncaughtExceptionError.hpp"

namespace redengine {
namespace error {

	UncaughtExceptionError::UncaughtExceptionError() {}

	UncaughtExceptionError::UncaughtExceptionError(const UncaughtExceptionError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ERROR(UncaughtExceptionError) {
		out << "Uncaught exception";
	}

}}
