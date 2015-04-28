#include "ThreadingError.hpp"

namespace redengine {
namespace platform {

	ThreadingError::ThreadingError() {}

	ThreadingError::ThreadingError(const ThreadingError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(ThreadingError)

}}
