#ifndef REDSTRAIN_MOD_VFS_ATTEMPTEDDIRECTORYLOOPERROR_HPP
#define REDSTRAIN_MOD_VFS_ATTEMPTEDDIRECTORYLOOPERROR_HPP

#include "VFS.hpp"
#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API AttemptedDirectoryLoopError : public VFSIOError {

	  private:
		const std::string path;

	  public:
		AttemptedDirectoryLoopError(const std::string& = "");
		AttemptedDirectoryLoopError(const VFS::Pathname&);
		AttemptedDirectoryLoopError(const VFS::PathIterator&, const VFS::PathIterator&);
		AttemptedDirectoryLoopError(const AttemptedDirectoryLoopError&);

		inline const std::string& getPath() const {
			return path;
		}

		REDSTRAIN_DECLARE_ERROR(AttemptedDirectoryLoopError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ATTEMPTEDDIRECTORYLOOPERROR_HPP */
