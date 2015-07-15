#ifndef REDSTRAIN_MOD_VFS_ISADIRECTORYERROR_HPP
#define REDSTRAIN_MOD_VFS_ISADIRECTORYERROR_HPP

#include "VFS.hpp"
#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API IsADirectoryError : public VFSIOError {

	  private:
		const std::string path;

	  public:
		IsADirectoryError(const std::string& = "");
		IsADirectoryError(const VFS::Pathname&);
		IsADirectoryError(const VFS::PathIterator&, const VFS::PathIterator&);
		IsADirectoryError(const IsADirectoryError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(IsADirectoryError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ISADIRECTORYERROR_HPP */
