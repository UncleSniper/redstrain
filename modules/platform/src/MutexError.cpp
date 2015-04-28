#include "MutexError.hpp"

namespace redengine {
namespace platform {

	MutexError::MutexError() {}

	MutexError::MutexError(const MutexError& error) : Error(error), ThreadingError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(MutexError)

}}
