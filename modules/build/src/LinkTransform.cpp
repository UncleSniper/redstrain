#include <redstrain/util/Delete.hpp>
#include <redstrain/util/NullAppender.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "Linker.hpp"
#include "BuildUI.hpp"
#include "BuildContext.hpp"
#include "LinkTransform.hpp"
#include "LinkerConfiguration.hpp"

using std::string;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::util::NullAppender;
using redengine::platform::Pathname;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	LinkTransform::LinkTransform(Linker& linker, Linkage::LinkMode mode, LinkerConfiguration& configuration)
			: linker(linker), mode(mode), configuration(configuration) {}


	LinkTransform::LinkTransform(const LinkTransform& transform) : ManyToOneTransform<FileArtifact>(transform),
			linker(transform.linker), mode(transform.mode), configuration(transform.configuration) {}

	struct LinkXformTargetSink : Appender<string> {

		LinkTransform& transform;
		BuildContext& context;
		Artifact& target;

		LinkXformTargetSink(LinkTransform& transform, BuildContext& context, Artifact& target)
				: transform(transform), context(context), target(target) {}

		virtual void append(const string&);

	};

	struct LinkXformSourceSink : Appender<string> {

		Linkage& linkage;

		LinkXformSourceSink(Linkage& linkage) : linkage(linkage) {}

		virtual void append(const string&);

	};

	void LinkTransform::perform(BuildContext& context, Artifact& target) {
		LinkXformTargetSink sink(*this, context, target);
		target.getFileReference("", sink, Artifact::FOR_OUTPUT, Artifact::FOR_USE, context);
	}

	void LinkXformTargetSink::append(const string& targetPath) {
		context.getUI().willPerformAction(BuildUI::ActionDescriptor(transform.getComponentType(),
				transform.getComponentName(), "linking", "",
				Pathname::stripPrefix(targetPath, transform.getComponentBaseDirectory())), false);
		Delete<Linkage> linkage(transform.getLinker().newLinkage(targetPath, transform.getLinkMode()));
		LinkXformSourceSink sink(**linkage);
		ManyToOneTransform<FileArtifact>::SourceIterator sbegin, send;
		transform.getSources(sbegin, send);
		for(; sbegin != send; ++sbegin)
			(*sbegin)->getFileReference("", sink, Artifact::FOR_INPUT, Artifact::FOR_USE, context);
		transform.getLinkerConfiguration().applyConfiguration(**linkage);
		linkage->invoke();
		target.notifyModified(context);
	}

	void LinkXformSourceSink::append(const string& sourcePath) {
		linkage.addSource(sourcePath);
	}

	struct PredictiveLinkXformTargetSink : Appender<string> {

		LinkTransform& transform;
		BuildContext& context;
		Artifact& target;

		PredictiveLinkXformTargetSink(LinkTransform& transform, BuildContext& context, Artifact& target)
				: transform(transform), context(context), target(target) {}

		virtual void append(const string&);

	};

	void LinkTransform::wouldPerform(BuildContext& context, Artifact& target) {
		PredictiveLinkXformTargetSink sink(*this, context, target);
		target.getFileReference("", sink, Artifact::FOR_OUTPUT, Artifact::FOR_PREDICTION, context);
	}

	void PredictiveLinkXformTargetSink::append(const string& targetPath) {
		context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(transform.getComponentType(),
				transform.getComponentName(), "would link", "",
				Pathname::stripPrefix(targetPath, transform.getComponentBaseDirectory())), false);
		NullAppender<string> sink;
		ManyToOneTransform<FileArtifact>::SourceIterator sbegin, send;
		transform.getSources(sbegin, send);
		for(; sbegin != send; ++sbegin)
			(*sbegin)->getFileReference("", sink, Artifact::FOR_INPUT, Artifact::FOR_PREDICTION, context);
		target.wouldModify(context);
	}

	void LinkTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "LinkTransform " << this << " {" << endln << shift;
		stream << indent << "mode = " << Linkage::getLinkModeName(mode) << endln;
		dumpManyToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
