#ifndef REDSTRAIN_MOD_PLATFORM_FILECLOSEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILECLOSEERROR_HPP

#include "FileError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FileCloseError : public FileError {

	  public:
		FileCloseError(File::Direction, File::ErrorCode);
		FileCloseError(const FileCloseError&);

		REDSTRAIN_DECLARE_ERROR(FileCloseError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILECLOSEERROR_HPP */
