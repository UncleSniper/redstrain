#ifndef REDSTRAIN_MOD_PLATFORM_FILEOPENERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILEOPENERROR_HPP

#include "FileError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FileOpenError : public FileError {

	  private:
		const std::string path;

	  public:
		FileOpenError(const std::string&, File::Direction, File::ErrorCode);
		FileOpenError(const FileOpenError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(FileOpenError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILEOPENERROR_HPP */
