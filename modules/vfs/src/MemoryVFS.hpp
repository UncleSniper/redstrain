#ifndef REDSTRAIN_MOD_VFS_MEMORYVFS_HPP
#define REDSTRAIN_MOD_VFS_MEMORYVFS_HPP

#include "MemoryBase.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MemoryVFS : public MemoryBase {

	  public:
		static const int DEFAULT_BASE_FLAGS = 0;

	  protected:
		MemoryVFS(MemoryDirectory*, int);
		MemoryVFS(const MemoryVFS&);

	  public:
		MemoryVFS(int = DEFAULT_BASE_FLAGS);

		virtual MemoryFile* createRegularFile(int);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_MEMORYVFS_HPP */
