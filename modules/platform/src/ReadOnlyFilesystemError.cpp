#include "ReadOnlyFilesystemError.hpp"

namespace redengine {
namespace platform {

	ReadOnlyFilesystemError::ReadOnlyFilesystemError(Filesystem::ErrorCode code) : FilesystemError(code) {}

	ReadOnlyFilesystemError::ReadOnlyFilesystemError(const ReadOnlyFilesystemError& error)
			: FilesystemError(error) {}

	REDSTRAIN_DEFINE_ERROR(ReadOnlyFilesystemError) {
		out << "Filesystem hierarchy not writable: " << Filesystem::getErrorMessage(code);
	}

}}
