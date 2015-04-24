#ifndef REDSTRAIN_MOD_PLATFORM_FILEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILEERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "File.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FileError : public error::IOError {

	  protected:
		const File::Direction direction;
		const File::ErrorCode code;

	  public:
		FileError(File::Direction, File::ErrorCode);
		FileError(const FileError&);

		inline File::Direction getDirection() const {
			return direction;
		}

		inline File::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(FileError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILEERROR_HPP */
