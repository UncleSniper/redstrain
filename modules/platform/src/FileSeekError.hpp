#ifndef REDSTRAIN_MOD_PLATFORM_FILESEEKERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILESEEKERROR_HPP

#include "FileError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FileSeekError : public FileError {

	  public:
		FileSeekError(File::Direction, File::ErrorCode);
		FileSeekError(const FileSeekError&);

		REDSTRAIN_DECLARE_ERROR(FileSeekError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILESEEKERROR_HPP */
