#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "FileArtifact.hpp"

using std::string;
using redengine::platform::Stat;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
#ifdef TESTING_REDSTRAIN_BUILD_API
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;
#endif /* TESTING_REDSTRAIN_BUILD_API */

namespace redengine {
namespace build {

	FileArtifact::FileArtifact(const string& pathname) : basename(pathname) {
		initializePredictedState();
	}

	FileArtifact::FileArtifact(const string& directory, const string& basename)
			: directory(directory), basename(basename) {
		initializePredictedState();
	}

	FileArtifact::FileArtifact(const FileArtifact& artifact)
			: Artifact(artifact), directory(artifact.directory), basename(artifact.basename) {}

	void FileArtifact::initializePredictedState() {
		if(isPresent())
			setPredictedModificationTimestamp(getModificationTimestamp());
	}

	string FileArtifact::getPathname() const {
		return Pathname::join(directory, basename);
	}

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

	string FileArtifact::getHumanReadableReference(bool asTarget) const {
		if(asTarget)
			return directory.empty() ? basename : directory;
		else
			return basename.empty() ? directory : basename;
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void FileArtifact::dumpArtifact(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "FileArtifact " << this << " {" << endln << shift;
		stream << indent << "directory = '" << directory << '\'' << endln;
		stream << indent << "basename = '" << basename << '\'' << endln;
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}
