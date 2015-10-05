#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "FileArtifact.hpp"
#include "BuildContext.hpp"
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
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	FileArtifact::FileArtifact(const string& path, const string& label) : path(path), label(label) {}

	FileArtifact::FileArtifact(const FileArtifact& artifact)
			: AbstractArtifact(artifact), path(artifact.path), label(artifact.label),
			intermediateDirectories(artifact.intermediateDirectories) {
		FileArtifactIterator idbegin(intermediateDirectories.begin()), idend(intermediateDirectories.end());
		for(; idbegin != idend; ++idbegin)
			(*idbegin)->ref();
	}

	FileArtifact::~FileArtifact() {
		FileArtifactIterator idbegin(intermediateDirectories.begin()), idend(intermediateDirectories.end());
		for(; idbegin != idend; ++idbegin)
			(*idbegin)->unref();
	}

	void FileArtifact::addIntermediateDirectories(BuildContext& context, const string& labelBase) {
		if(!intermediateDirectories.empty())
			return;
		string lbase(Pathname::tidy(labelBase));
		string current(Pathname::tidy(path));
		while(current != lbase) {
			FileArtifact& idir = context.internFileArtifact(current, Pathname::stripPrefix(current, lbase));
			if(intermediateDirectories.append(&idir))
				idir.ref();
			current = Pathname::dirname(current, Pathname::LOGICAL);
		}
	}

	void FileArtifact::notifyIntermediateDirectoriesCreated(const Mood& mood, BuildContext& context) const {
		FileArtifactIterator idbegin(intermediateDirectories.begin()), idend(intermediateDirectories.end());
		for(; idbegin != idend; ++idbegin)
			mood.modified(**idbegin, context);
	}

	void FileArtifact::wouldModify(BuildContext& context) {
		AbstractArtifact::wouldModify(context);
		notifyIntermediateDirectoriesCreated(PredictiveMood::instance, context);
	}

	bool FileArtifact::isPresent() {
		return Filesystem::access(path, Filesystem::FILE_EXISTS);
	}

	void FileArtifact::getModificationTimestamp(Appender<time_t>& sink) {
		Stat stat;
		Filesystem::stat(path, stat);
		sink.append(stat.getModificationTimestamp());
	}

	void FileArtifact::notifyModified(BuildContext& context) {
		AbstractArtifact::notifyModified(context);
		notifyIntermediateDirectoriesCreated(DefinitiveMood::instance, context);
	}

	void FileArtifact::remove() {
		if(Filesystem::access(path, Filesystem::FILE_EXISTS))
			Filesystem::removeRecursively(path);
	}

	InputStream<char>* FileArtifact::getInputStream(BuildContext&) {
		return new FileInputStream(path);
	}

	OutputStream<char>* FileArtifact::getOutputStream(BuildContext& context, ReferenceMood mood) {
		getFileReferenceForOutput(mood, context);
		return mood == FOR_USE ? new FileOutputStream(path) : NULL;
	}

	void FileArtifact::getFileReferenceForOutput(ReferenceMood mood, BuildContext& context) const {
		string lbase(Pathname::stripSuffix(path, label));
		string dirpath(Pathname::dirname(path, Pathname::LOGICAL));
		if(mood == FOR_USE) {
			if(!Filesystem::access(dirpath, Filesystem::FILE_EXISTS)) {
				context.getUI().willPerformAction(BuildUI::ActionDescriptor("", "",
						"creating directory", "", Pathname::stripPrefix(dirpath, lbase)), true);
				Filesystem::mkdirRecursive(dirpath);
				notifyIntermediateDirectoriesCreated(DefinitiveMood::instance, context);
			}
		}
		else {
			context.getUI().wouldPerformAction(BuildUI::ActionDescriptor("", "",
					"would create directory", "", Pathname::stripPrefix(dirpath, lbase)), true);
			notifyIntermediateDirectoriesCreated(PredictiveMood::instance, context);
		}
	}

	void FileArtifact::getFileReference(const string& suffix, Appender<string>& sink, ReferenceDirection direction,
			ReferenceMood mood, BuildContext& context) {
		if(suffix.empty()) {
			if(direction == FOR_OUTPUT)
				getFileReferenceForOutput(mood, context);
			sink.append(path);
		}
		else if(Pathname::endsWith(Pathname::tidy(path), Pathname::tidy(suffix))) {
			if(direction == FOR_OUTPUT)
				getFileReferenceForOutput(mood, context);
			sink.append(Pathname::stripSuffix(path, suffix));
		}
		else {
			ArtifactStage& stage = getEffectiveArtifactStage();
			if(direction == FOR_INPUT) {
				if(Filesystem::access(path, Filesystem::FILE_EXISTS))
					stage.stage(*this, suffix, true, context);
			}
			else {
				stage.stage(*this, suffix, false, context);
				notifyIntermediateDirectoriesCreated(DefinitiveMood::instance, context);
			}
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

	void FileArtifact::dumpArtifact(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "FileArtifact " << this << " {" << endln << shift;
		stream << indent << "path = '" << path << '\'' << endln;
		stream << indent << "label = '" << label << '\'' << endln;
		stream << indent << "intermediateDirectories = {" << endln << shift;
		FileArtifactIterator fabegin(intermediateDirectories.begin()), faend(intermediateDirectories.end());
		for(; fabegin != faend; ++fabegin)
			(*fabegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		dumpArtifactAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
