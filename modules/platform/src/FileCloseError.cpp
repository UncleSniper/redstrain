#include "FileCloseError.hpp"

namespace redengine {
namespace platform {

	FileCloseError::FileCloseError(File::Direction direction, File::ErrorCode code) : FileError(direction, code) {}

	FileCloseError::FileCloseError(const FileCloseError& error) : Error(error), FileError(error) {}

	REDSTRAIN_DEFINE_ERROR(FileCloseError) {
		out << "Failed to close file open for ";
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
