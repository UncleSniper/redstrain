#include "FileOpenError.hpp"

using std::string;

namespace redengine {
namespace platform {

	FileOpenError::FileOpenError(const string& path, File::Direction direction, File::ErrorCode code)
			: FileError(direction, code), path(path) {}

	FileOpenError::FileOpenError(const FileOpenError& error) : Error(error), FileError(error), path(error.path) {}

	FileOpenError::~FileOpenError() {}

	REDSTRAIN_DEFINE_ERROR(FileOpenError) {
		out << "Failed to open file";
		if(!path.empty())
			out << " '" << path << '\'';
		out << " for ";
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
