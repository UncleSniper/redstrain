#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "FileArtifact.hpp"

using std::string;
using redengine::platform::Stat;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;

namespace redengine {
namespace build {

	FileArtifact::FileArtifact(const string& pathname) : basename(pathname) {}

	FileArtifact::FileArtifact(const string& directory, const string& basename)
			: directory(directory), basename(basename) {}

	FileArtifact::FileArtifact(const FileArtifact& artifact)
			: Artifact(artifact), directory(artifact.directory), basename(artifact.basename) {}

	bool FileArtifact::isPresent() {
		return Filesystem::access(Pathname::join(directory, basename), Filesystem::FILE_EXISTS);
	}

	time_t FileArtifact::getModificationTimestamp() {
		Stat info;
		Filesystem::stat(Pathname::join(directory, basename), info);
		return info.getModificationTimestamp();
	}

	void FileArtifact::remove() {
		string pathname(Pathname::join(directory, basename));
		if(Filesystem::access(pathname, Filesystem::FILE_EXISTS))
			Filesystem::removeRecursively(pathname);
	}

}}
