#include "FileIOError.hpp"

namespace redengine {
namespace platform {

	FileIOError::FileIOError(File::Direction direction, File::ErrorCode code) : FileError(direction, code) {}

	FileIOError::FileIOError(const FileIOError& error) : Error(error), FileError(error) {}

	REDSTRAIN_DEFINE_ERROR(FileIOError) {
		out << "Failed to ";
		switch(direction) {
			case File::INPUT:
				out << "read";
				break;
			case File::OUTPUT:
				out << "write";
				break;
			default:
				out << "read/write";
				break;
		}
		out << " file: " << File::getErrorMessage(code);
	}

}}
