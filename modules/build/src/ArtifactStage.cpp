#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/io/FileOutputStream.hpp>

#include "Artifact.hpp"
#include "ArtifactStage.hpp"

using std::string;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::StreamCloser;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
using redengine::io::FileOutputStream;

namespace redengine {
namespace build {

	ArtifactStage::ArtifactStage(const string& directory)
			: directory(directory), label(directory) {}

	ArtifactStage::ArtifactStage(const string& directory, const string& label)
			: directory(directory), label(label) {}

	ArtifactStage::ArtifactStage(const ArtifactStage& stage)
			: ReferenceCounted(stage), directory(stage.directory), label(stage.label) {}

	void ArtifactStage::setDirectory(const string& directory) {
		this->directory = directory;
	}

	void ArtifactStage::setLabel(const string& label) {
		this->label = label;
	}

	void ArtifactStage::stage(Artifact& artifact, const string& suffix) {
		string fullPath(Pathname::join(directory, suffix));
		Filesystem::mkdirRecursive(Pathname::dirname(fullPath, Pathname::LOGICAL));
		Delete<InputStream<char> > in(artifact.getInputStream());
		StreamCloser inCloser(*in);
		FileOutputStream out(fullPath);
		StreamCloser outCloser(out);
		in->copyTo(out);
		inCloser.close();
		outCloser.close();
	}

}}
