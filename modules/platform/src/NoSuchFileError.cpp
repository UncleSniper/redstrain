#include "NoSuchFileError.hpp"

namespace redengine {
namespace platform {

	NoSuchFileError::NoSuchFileError(Filesystem::ErrorCode code) : FilesystemError(code) {}

	NoSuchFileError::NoSuchFileError(const NoSuchFileError& error) : Error(error), FilesystemError(error) {}

	REDSTRAIN_DEFINE_ERROR(NoSuchFileError) {
		out << "No such file (" << Filesystem::getErrorMessage(code) << ')';
	}

}}
