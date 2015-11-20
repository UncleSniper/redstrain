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
			case File::RANDOM_ACCESS:
				out << "reading and writing";
				break;
			default:
				out << "unknown access";
				break;
		}
		out << ": " << File::getErrorMessage(code);
	}

}}
