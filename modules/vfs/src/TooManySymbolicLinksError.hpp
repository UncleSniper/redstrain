#ifndef REDSTRAIN_MOD_VFS_TOOMANYSYMBOLICLINKSERROR_HPP
#define REDSTRAIN_MOD_VFS_TOOMANYSYMBOLICLINKSERROR_HPP

#include "VFS.hpp"
#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API TooManySymbolicLinksError : public VFSIOError {

	  private:
		const std::string path;

	  public:
		TooManySymbolicLinksError(const std::string& = "");
		TooManySymbolicLinksError(const VFS::Pathname&);
		TooManySymbolicLinksError(const VFS::PathIterator&, const VFS::PathIterator&);
		TooManySymbolicLinksError(const TooManySymbolicLinksError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(TooManySymbolicLinksError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_TOOMANYSYMBOLICLINKSERROR_HPP */
