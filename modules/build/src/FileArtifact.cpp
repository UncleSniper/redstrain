#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "FileArtifact.hpp"
#include "ArtifactStage.hpp"

using std::string;
using redengine::util::Appender;
using redengine::platform::Stat;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::platform::Filesystem;
using redengine::io::DefaultConfiguredOutputStream;

namespace redengine {
namespace build {

	FileArtifact::FileArtifact(const string& path, const string& label) : path(path), label(label) {}

	FileArtifact::FileArtifact(const FileArtifact& artifact)
			: AbstractArtifact(artifact), path(artifact.path), label(artifact.label) {}

	bool FileArtifact::isPresent() {
		return Filesystem::access(path, Filesystem::FILE_EXISTS);
	}

	void FileArtifact::getModificationTimestamp(Appender<time_t>& sink) {
		Stat stat;
		Filesystem::stat(path, stat);
		sink.append(stat.getModificationTimestamp());
	}

	void FileArtifact::remove() {
		if(Filesystem::access(path, Filesystem::FILE_EXISTS))
			Filesystem::removeRecursively(path);
	}

	InputStream<char>* FileArtifact::getInputStream() {
		return new FileInputStream(path);
	}

	OutputStream<char>* FileArtifact::getOutputStream() {
		Filesystem::mkdirRecursive(Pathname::dirname(path, Pathname::LOGICAL));
		return new FileOutputStream(path);
	}

	void FileArtifact::getFileReference(const string& suffix, Appender<string>& sink, ReferenceDirection direction) {
		if(suffix.empty()) {
			if(direction == FOR_OUTPUT)
				Filesystem::mkdirRecursive(Pathname::dirname(path, Pathname::LOGICAL));
			sink.append(path);
		}
		else if(Pathname::endsWith(Pathname::tidy(path), Pathname::tidy(suffix))) {
			if(direction == FOR_OUTPUT)
				Filesystem::mkdirRecursive(Pathname::dirname(path, Pathname::LOGICAL));
			sink.append(Pathname::stripSuffix(path, suffix));
		}
		else {
			ArtifactStage& stage = getEffectiveArtifactStage();
			if(direction == FOR_INPUT) {
				if(Filesystem::access(path, Filesystem::FILE_EXISTS))
					stage.stage(*this, suffix, true);
			}
			else
				stage.stage(*this, suffix, false);
			sink.append(stage.getDirectory());
		}
	}

	bool FileArtifact::isTransformable() {
		return true;
	}

	string FileArtifact::getName() {
		return path;
	}

	string FileArtifact::getLabel() {
		return label;
	}

	void FileArtifact::dumpArtifact(DefaultConfiguredOutputStream<char>::Stream&) const {
		//TODO
	}

}}
