#ifndef REDSTRAIN_MOD_VFS_FILEURISCHEME_HPP
#define REDSTRAIN_MOD_VFS_FILEURISCHEME_HPP

#include "URIAcquirer.hpp"
#include "HierarchicalURIScheme.hpp"

namespace redengine {
namespace vfs {

	class FileURI;

	class REDSTRAIN_VFS_API FileURIScheme : public HierarchicalURIScheme {

	  public:
		static const int FL_MANAGE_VFS             = 01;
		static const int FL_MANAGE_REMOTE_ACQUIRER = 02;
		static const int FL_MASK                   = 03;

	  private:
		VFS* vfs;
		URIAcquirer<FileURI>* remoteAcquirer;
		int flags;

	  protected:
		virtual URI* newURI(const text::String16&, const HierarchicalURI::Decomposition&) const;

	  public:
		FileURIScheme(VFS* = NULL, URIAcquirer<FileURI>* = NULL, int = 0);
		FileURIScheme(const FileURIScheme&);
		virtual ~FileURIScheme();

		inline VFS* getVFS() const {
			return vfs;
		}

		void setVFS(VFS*);

		inline URIAcquirer<FileURI>* getRemoteAcquirer() const {
			return remoteAcquirer;
		}

		void setRemoteAcquirer(URIAcquirer<FileURI>*);

		inline int getFlags() const {
			return flags;
		}

		inline void setFlags(int flags) {
			this->flags = flags & FileURIScheme::FL_MASK;
		}

		inline void addFlags(int mask) {
			flags |= mask & FileURIScheme::FL_MASK;
		}

		inline void removeFlags(int mask) {
			flags &= ~mask;
		}

	};

}}

#endif /* REDSTRAIN_MOD_VFS_FILEURISCHEME_HPP */
