#ifndef REDSTRAIN_MOD_VFS_NOSUCHFILEERROR_HPP
#define REDSTRAIN_MOD_VFS_NOSUCHFILEERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API NoSuchFileError : public VFSIOError {

	  private:
		const std::string& path;

	  public:
		NoSuchFileError(const std::string& = "");
		NoSuchFileError(const NoSuchFileError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(NoSuchFileError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_NOSUCHFILEERROR_HPP */
