#ifndef REDSTRAIN_MOD_VFS_NOTADIRECTORYERROR_HPP
#define REDSTRAIN_MOD_VFS_NOTADIRECTORYERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API NotADirectoryError : public VFSIOError {

	  private:
		const std::string path;

	  public:
		NotADirectoryError(const std::string& = "");
		NotADirectoryError(const NotADirectoryError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(NotADirectoryError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_NOTADIRECTORYERROR_HPP */
