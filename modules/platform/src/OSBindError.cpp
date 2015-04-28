#include "OSBindError.hpp"

namespace redengine {
namespace platform {

	OSBindError::OSBindError() {}

	OSBindError::OSBindError(const OSBindError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(OSBindError)

}}
