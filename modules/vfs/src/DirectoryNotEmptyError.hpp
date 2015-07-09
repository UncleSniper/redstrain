#ifndef REDSTRAIN_MOD_VFS_DIRECTORYNOTEMPTYERROR_HPP
#define REDSTRAIN_MOD_VFS_DIRECTORYNOTEMPTYERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API DirectoryNotEmptyError : public VFSIOError {

	  private:
		const std::string& path;

	  public:
		DirectoryNotEmptyError(const std::string& = "");
		DirectoryNotEmptyError(const DirectoryNotEmptyError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(DirectoryNotEmptyError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_DIRECTORYNOTEMPTYERROR_HPP */
