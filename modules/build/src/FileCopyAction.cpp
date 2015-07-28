#include <redstrain/platform/Stat.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/platform/Filesystem.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "BuildUI.hpp"
#include "Component.hpp"
#include "FileArtifact.hpp"
#include "FileCopyAction.hpp"

using std::string;
using redengine::platform::Stat;
using redengine::io::StreamCloser;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
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

	FileCopyAction::FileCopyAction(FileArtifact& source, FileArtifact& destination, bool preservePermissions)
			: source(source), destination(destination), preservePermissions(preservePermissions) {
		source.ref();
		destination.ref();
	}

	FileCopyAction::FileCopyAction(const FileCopyAction& action) : Action(action), FileGeneratingAction(action),
			source(action.source), destination(action.destination),
			preservePermissions(action.preservePermissions) {
		source.ref();
		destination.ref();
	}

	FileCopyAction::~FileCopyAction() {
		source.unref();
		destination.unref();
	}

	void FileCopyAction::addIntermediateDirectories(const Component& component, BuildContext& context) {
		addIntermediateDirectoriesFor(component.getBaseDirectory(),
				Pathname::stripPrefix(Pathname::dirname(destination.getPathname()),
				component.getBaseDirectory()), context);
	}

	void FileCopyAction::perform(BuildContext&) {
		createIntermediateDirectories();
		string srcpath(source.getPathname()), destpath(destination.getPathname());
		FileInputStream fis(srcpath);
		StreamCloser inclose(fis);
		FileOutputStream fos(destpath);
		StreamCloser outclose(fos);
		fis.copyTo(fos);
		inclose.close();
		outclose.close();
		if(preservePermissions) {
			Stat stat;
			Filesystem::stat(srcpath, stat);
			Filesystem::chmod(destpath, stat.getPermissions());
		}
	}

	void FileCopyAction::wouldPerform(BuildContext&) {
		if(source.wouldBePresent()) {
			wouldCreateIntermediateDirectories();
			destination.wouldModify();
		}
	}

	void FileCopyAction::notifyUIWillPerform(BuildUI& ui) const {
		ui.willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(), "copying",
				source.getBasename(), Pathname::stripPrefix(destination.getDirectory(),
				getComponentBaseDirectory())));
	}

	void FileCopyAction::notifyUIWouldPerform(BuildUI& ui) const {
		ui.wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(), "would copy",
				source.getBasename(), Pathname::stripPrefix(destination.getDirectory(),
				getComponentBaseDirectory())));
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void FileCopyAction::dumpAction(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "FileCopyAction " << this << " {" << endln << shift;
		stream << indent << "source ->" << endln << shift;
		source.dumpArtifact(stream);
		stream << unshift;
		stream << indent << "destination ->" << endln << shift;
		destination.dumpArtifact(stream);
		stream << unshift;
		stream << indent << "preservePermissions = " << (preservePermissions ? "true" : "false") << endln;
		dumpFileGeneratingActionAspects(stream);
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}
