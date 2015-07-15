#include "MemoryVFS.hpp"

namespace redengine {
namespace vfs {

	MemoryVFS::MemoryVFS(MemoryDirectory* root, int baseFlags) : MemoryBase(root, baseFlags) {}

	MemoryVFS::MemoryVFS(int baseFlags) : MemoryBase(NULL, baseFlags) {}

	MemoryVFS::MemoryVFS(const MemoryVFS& vfs) : MemoryBase(vfs) {}

	MemoryBase::MemoryFile* MemoryVFS::createRegularFile(int) {
		//TODO
		return NULL;
	}

}}
