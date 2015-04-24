#ifndef REDSTRAIN_MOD_PLATFORM_FILEIOERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILEIOERROR_HPP

#include "FileError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FileIOError : public FileError {

	  public:
		FileIOError(File::Direction, File::ErrorCode);
		FileIOError(const FileIOError&);

		REDSTRAIN_DECLARE_ERROR(FileIOError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILEIOERROR_HPP */
