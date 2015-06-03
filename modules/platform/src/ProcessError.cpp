#include "ProcessError.hpp"

namespace redengine {
namespace platform {

	ProcessError::ProcessError() {}

	ProcessError::ProcessError(const ProcessError& error) : Error(error), IOError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(ProcessError)

}}
