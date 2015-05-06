#include "FileOpenError.hpp"

namespace redengine {
namespace platform {

	FileOpenError::FileOpenError(File::Direction direction, File::ErrorCode code) : FileError(direction, code) {}

	FileOpenError::FileOpenError(const FileOpenError& error) : Error(error), FileError(error) {}

	REDSTRAIN_DEFINE_ERROR(FileOpenError) {
		out << "Failed to open file for ";
		switch(direction) {
			case File::INPUT:
				out << "reading";
				break;
			case File::OUTPUT:
				out << "writing";
				break;
			default:
				out << "reading and writing";
				break;
		}
		out << ": " << File::getErrorMessage(code);
	}

}}
