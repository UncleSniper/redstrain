#include "ReadOnlyFilesystemError.hpp"

namespace redengine {
namespace vfs {

	ReadOnlyFilesystemError::ReadOnlyFilesystemError() {}

	ReadOnlyFilesystemError::ReadOnlyFilesystemError(const ReadOnlyFilesystemError& error)
			: Error(error), VFSIOError(error) {}

	REDSTRAIN_DEFINE_ERROR(ReadOnlyFilesystemError) {
		out << "Filesystem is read-only";
	}

}}
