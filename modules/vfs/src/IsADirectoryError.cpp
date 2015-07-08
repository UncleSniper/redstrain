#include "IsADirectoryError.hpp"

namespace redengine {
namespace vfs {

	IsADirectoryError::IsADirectoryError() {}

	IsADirectoryError::IsADirectoryError(const IsADirectoryError& error) : Error(error), VFSIOError(error) {}

	REDSTRAIN_DEFINE_ERROR(IsADirectoryError) {
		out << "Is a directory";
	}

}}
