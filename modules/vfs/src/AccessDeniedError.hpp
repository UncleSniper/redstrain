#ifndef REDSTRAIN_MOD_VFS_ACCESSDENIEDERROR_HPP
#define REDSTRAIN_MOD_VFS_ACCESSDENIEDERROR_HPP

#include "VFS.hpp"
#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API AccessDeniedError : public VFSIOError {

	  private:
		const std::string path;

	  public:
		AccessDeniedError(const std::string& = "");
		AccessDeniedError(const VFS::Pathname&);
		AccessDeniedError(const VFS::PathIterator&, const VFS::PathIterator&);
		AccessDeniedError(const AccessDeniedError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(AccessDeniedError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ACCESSDENIEDERROR_HPP */
