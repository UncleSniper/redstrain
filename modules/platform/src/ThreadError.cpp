#include "ThreadError.hpp"

namespace redengine {
namespace platform {

	ThreadError::ThreadError() {}

	ThreadError::ThreadError(const ThreadError& error) : Error(error), ThreadingError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(ThreadError)

}}
