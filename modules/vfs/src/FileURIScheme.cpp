#include "VFS.hpp"
#include "FileURI.hpp"
#include "FileURIScheme.hpp"

using redengine::text::String16;

namespace redengine {
namespace vfs {

	FileURIScheme::FileURIScheme(VFS* vfs, URIAcquirer<FileURI>* remoteAcquirer, int flags)
			: vfs(vfs), remoteAcquirer(remoteAcquirer), flags(flags & FileURIScheme::FL_MASK) {}

	FileURIScheme::FileURIScheme(const FileURIScheme& scheme) : HierarchicalURIScheme(scheme),
			vfs(scheme.vfs), remoteAcquirer(scheme.remoteAcquirer), flags(0) {}

	FileURIScheme::~FileURIScheme() {
		if(vfs && (flags & FileURIScheme::FL_MANAGE_VFS))
			delete vfs;
		if(remoteAcquirer && (flags & FileURIScheme::FL_MANAGE_REMOTE_ACQUIRER))
			delete remoteAcquirer;
	}

	URI* FileURIScheme::newURI(const String16& uri, const HierarchicalURI::Decomposition& decomposition) const {
		FileURI* file = new FileURI(uri, decomposition);
		file->setVFS(vfs);
		file->setRemoteAcquirer(remoteAcquirer);
		return file;
	}

}}
