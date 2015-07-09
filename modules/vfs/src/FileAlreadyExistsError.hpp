#ifndef REDSTRAIN_MOD_VFS_FILEALREADYEXISTSERROR_HPP
#define REDSTRAIN_MOD_VFS_FILEALREADYEXISTSERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API FileAlreadyExistsError : public VFSIOError {

	  private:
		const std::string path;

	  public:
		FileAlreadyExistsError(const std::string& = "");
		FileAlreadyExistsError(const FileAlreadyExistsError&);


		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(FileAlreadyExistsError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_FILEALREADYEXISTSERROR_HPP */
