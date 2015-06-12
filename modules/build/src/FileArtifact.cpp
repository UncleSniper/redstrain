#include <redstrain/platform/Filesystem.hpp>

#include "FileArtifact.hpp"

using std::string;
using redengine::platform::Stat;
using redengine::platform::Filesystem;

namespace redengine {
namespace build {

	FileArtifact::FileArtifact(const string& pathname) : pathname(pathname) {}

	FileArtifact::FileArtifact(const FileArtifact& artifact) : Artifact(artifact), pathname(artifact.pathname) {}

	bool FileArtifact::isPresent() {
		return Filesystem::access(pathname, Filesystem::FILE_EXISTS);
	}

	time_t FileArtifact::getModificationTimestamp() {
		Stat info;
		Filesystem::stat(pathname, info);
		return info.getModificationTimestamp();
	}

	void FileArtifact::remove() {
		if(Filesystem::access(pathname, Filesystem::FILE_EXISTS))
			Filesystem::removeRecursively(pathname);
	}

}}
