#include "FileAccessDeniedError.hpp"

namespace redengine {
namespace platform {

	FileAccessDeniedError::FileAccessDeniedError(Filesystem::ErrorCode code) : FilesystemError(code) {}

	FileAccessDeniedError::FileAccessDeniedError(const FileAccessDeniedError& error) : FilesystemError(error) {}

	REDSTRAIN_DEFINE_ERROR(FileAccessDeniedError) {
		out << "Access to file was denied: " << Filesystem::getErrorMessage(code);
	}

}}
