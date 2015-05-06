#include "FilesystemError.hpp"

namespace redengine {
namespace platform {

	FilesystemError::FilesystemError(Filesystem::ErrorCode code) : code(code) {}

	FilesystemError::FilesystemError(const FilesystemError& error)
			: Error(error), IOError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(FilesystemError) {
		out << "Filesystem operation failed: " << Filesystem::getErrorMessage(code);
	}

}}
