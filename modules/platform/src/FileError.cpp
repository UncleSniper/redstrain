#include "FileError.hpp"

namespace redengine {
namespace platform {

	FileError::FileError(File::Direction direction, File::ErrorCode code) : direction(direction), code(code) {}

	FileError::FileError(const FileError& error) : IOError(error), direction(error.direction), code(error.code) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(FileError)

}}
