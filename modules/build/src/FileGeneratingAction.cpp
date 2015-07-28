#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "FileArtifact.hpp"
#include "BuildContext.hpp"
#include "FileGeneratingAction.hpp"

using std::string;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	FileGeneratingAction::FileGeneratingAction() {}

	FileGeneratingAction::FileGeneratingAction(const FileGeneratingAction& action)
			: Action(action), intermediates(action.intermediates) {
		FileArtifactIterator begin(intermediates.begin()), end(intermediates.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	FileGeneratingAction::~FileGeneratingAction() {
		FileArtifactIterator begin(intermediates.begin()), end(intermediates.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	bool FileGeneratingAction::addIntermediateDirectory(FileArtifact* file) {
		if(!file)
			return false;
		if(!intermediates.insert(file).second)
			return false;
		file->ref();
		return true;
	}

	bool FileGeneratingAction::removeIntermediateDirectory(FileArtifact* file) {
		if(!intermediates.erase(file))
			return false;
		file->unref();
		return true;
	}

	void FileGeneratingAction::clearIntermediateDirectories() {
		FileArtifactIterator begin(intermediates.begin()), end(intermediates.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		intermediates.clear();
	}

	void FileGeneratingAction::getIntermediateDirectories(FileArtifactIterator& begin,
			FileArtifactIterator& end) const {
		begin = intermediates.begin();
		end = intermediates.end();
	}

	void FileGeneratingAction::addIntermediateDirectoriesFor(const string& directory, const string& basename,
			BuildContext& context) {
		string absdir(Pathname::tidy(Pathname::join(Pathname::getWorkingDirectory(), directory)));
		string absbn(Pathname::tidy(Pathname::join(Pathname::getWorkingDirectory(),
				Pathname::join(directory, basename))));
		if(!Pathname::startsWith(absbn, absdir))
			return;
		for(; absbn != absdir; absbn = Pathname::dirname(absbn))
			addIntermediateDirectory(context.internFileArtifact(directory, Pathname::stripPrefix(absbn, absdir)));
	}

	void FileGeneratingAction::addIntermediateDirectoriesFor(const FileArtifact& file, BuildContext& context) {
		addIntermediateDirectoriesFor(file.getDirectory(), file.getBasename(), context);
	}

	void FileGeneratingAction::createIntermediateDirectories() const {
		FileArtifactIterator begin(intermediates.begin()), end(intermediates.end());
		for(; begin != end; ++begin)
			Filesystem::mkdirRecursive((*begin)->getPathname());
	}

	void FileGeneratingAction::wouldCreateIntermediateDirectories(BuildContext& context) const {
		FileArtifactIterator begin(intermediates.begin()), end(intermediates.end());
		for(; begin != end; ++begin)
			(*begin)->wouldModify(context);
	}

	void FileGeneratingAction::dumpFileGeneratingActionAspects(DefaultConfiguredOutputStream<char>::Stream&
			stream) const {
		stream << indent << "intermediates = {" << endln << shift;
		FileArtifactIterator begin(intermediates.begin()), end(intermediates.end());
		for(; begin != end; ++begin)
			(*begin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
