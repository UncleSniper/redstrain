#include <redstrain/util/Delete.hpp>

#include "Linker.hpp"
#include "LinkTransform.hpp"
#include "LinkerConfiguration.hpp"

using std::string;
using redengine::util::Delete;
using redengine::util::Appender;
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

	void LinkXformTargetSink::append(const string& targetPath) {
		Delete<Linkage> linkage(transform.getLinker().newLinkage(targetPath, transform.getLinkMode()));
		LinkXformSourceSink sink(**linkage);
		ManyToOneTransform<FileArtifact>::SourceIterator sbegin, send;
		transform.getSources(sbegin, send);
		for(; sbegin != send; ++sbegin)
			(*sbegin)->getFileReference("", sink, Artifact::FOR_INPUT, context);
		transform.getLinkerConfiguration().applyConfiguration(**linkage);
		linkage->invoke();
		target.notifyModified(context);
	}

	void LinkXformSourceSink::append(const string& sourcePath) {
		linkage.addSource(sourcePath);
	}

	void LinkTransform::perform(BuildContext& context, Artifact& target) {
		LinkXformTargetSink sink(*this, context, target);
		target.getFileReference("", sink, Artifact::FOR_OUTPUT, context);
	}

	void LinkTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "LinkTransform " << this << " {" << endln << shift;
		stream << indent << "mode = " << Linkage::getLinkModeName(mode) << endln;
		dumpManyToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
