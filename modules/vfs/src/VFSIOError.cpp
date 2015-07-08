#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	VFSIOError::VFSIOError() {}

	VFSIOError::VFSIOError(const VFSIOError& error) : Error(error), VFSError(error), IOError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(VFSIOError)

}}
