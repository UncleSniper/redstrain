#ifndef REDSTRAIN_MOD_PLATFORM_FILEOPENERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILEOPENERROR_HPP

#include "FileError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FileOpenError : public FileError {

	  public:
		FileOpenError(File::Direction, File::ErrorCode);
		FileOpenError(const FileOpenError&);

		REDSTRAIN_DECLARE_ERROR(FileOpenError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILEOPENERROR_HPP */
