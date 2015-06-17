#ifndef REDSTRAIN_MOD_VFS_UNLINKER_HPP
#define REDSTRAIN_MOD_VFS_UNLINKER_HPP

#include "VFS.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API Unlinker {

	  private:
		VFS& fs;
		VFS::PathIterator pathBegin, pathEnd;
		bool directory;

	  public:
		Unlinker(VFS&, const VFS::Pathname&, bool);
		Unlinker(VFS&, VFS::PathIterator, VFS::PathIterator, bool);
		Unlinker(const Unlinker&);
		~Unlinker();

		inline VFS& getFilesystem() {
			return fs;
		}

		inline const VFS& getFilesystem() const {
			return fs;
		}

		inline const VFS::PathIterator& getPathBegin() const {
			return pathBegin;
		}

		inline const VFS::PathIterator& getPathEnd() const {
			return pathEnd;
		}

		void getPath(VFS::PathIterator&, VFS::PathIterator&) const;

		inline bool isDirectory() const {
			return directory;
		}

		void unlink();
		void release();

		Unlinker& operator=(const VFS::Pathname&);
		Unlinker& operator=(const Unlinker&);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_UNLINKER_HPP */
