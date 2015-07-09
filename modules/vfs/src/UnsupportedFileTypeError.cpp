#include "UnsupportedFileTypeError.hpp"

namespace redengine {
namespace vfs {

	UnsupportedFileTypeError::UnsupportedFileTypeError(Stat::Type type) : type(type) {}

	UnsupportedFileTypeError::UnsupportedFileTypeError(const UnsupportedFileTypeError& error)
			: Error(error), VFSIOError(error), type(error.type) {}

	REDSTRAIN_DEFINE_ERROR(UnsupportedFileTypeError) {
		out << "File type ";
		switch(type) {
			case Stat::REGULAR_FILE:
				out << "'regular file'";
				break;
			case Stat::DIRECTORY:
				out << "'directory'";
				break;
			case Stat::CHARACTER_DEVICE:
				out << "'character device'";
				break;
			case Stat::BLOCK_DEVICE:
				out << "'block device'";
				break;
			case Stat::NAMED_PIPE:
				out << "'named pipe'";
				break;
			case Stat::SYMBOLIC_LINK:
				out << "'symbolic link'";
				break;
			case Stat::LOCAL_SOCKET:
				out << "'local socket'";
				break;
			default:
				out << "<unknown type>";
				break;
		}
		out << " is not supported";
	}

}}
