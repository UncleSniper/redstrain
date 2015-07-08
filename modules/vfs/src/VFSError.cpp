#include "VFSError.hpp"

namespace redengine {
namespace vfs {

	VFSError::VFSError() {}

	VFSError::VFSError(const VFSError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(VFSError)

}}
