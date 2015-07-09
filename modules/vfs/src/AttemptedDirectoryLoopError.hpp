#ifndef REDSTRAIN_MOD_VFS_ATTEMPTEDDIRECTORYLOOPERROR_HPP
#define REDSTRAIN_MOD_VFS_ATTEMPTEDDIRECTORYLOOPERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API AttemptedDirectoryLoopError : public VFSIOError {

	  private:
		const std::string path;

	  public:
		AttemptedDirectoryLoopError(const std::string& = "");
		AttemptedDirectoryLoopError(const AttemptedDirectoryLoopError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(AttemptedDirectoryLoopError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ATTEMPTEDDIRECTORYLOOPERROR_HPP */
