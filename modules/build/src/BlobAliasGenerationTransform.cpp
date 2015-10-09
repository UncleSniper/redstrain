#include <redstrain/util/Delete.hpp>
#include <redstrain/vfs/BlobVFS.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "BuildContext.hpp"
#include "BlobAliasGenerationTransform.hpp"

using redengine::util::Delete;
using redengine::vfs::BlobVFS;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	BlobAliasGenerationTransform::BlobAliasGenerationTransform(FileArtifact& source,
			BlobAliasLanguage::AliasConfiguration* configuration) : OneToOneTransform<FileArtifact>(source),
			configuration(configuration) {
		if(configuration)
			configuration->ref();
	}

	BlobAliasGenerationTransform::BlobAliasGenerationTransform(const BlobAliasGenerationTransform& transform)
			: OneToOneTransform<FileArtifact>(transform), configuration(transform.configuration) {
		if(configuration)
			configuration->ref();
	}

	BlobAliasGenerationTransform::~BlobAliasGenerationTransform() {
		if(configuration)
			configuration->unref();
	}

	void BlobAliasGenerationTransform::perform(BuildContext& context, Artifact& target) {
		context.getUI().willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"generating", getSource().getLabel(), target.getLabel()), false);
		Delete<InputStream<char> > in(getSource().getInputStream(context));
		StreamCloser inCloser(*in);
		Delete<OutputStream<char> > out(target.getOutputStream(context, Artifact::FOR_USE, this));
		StreamCloser outCloser(*out);
		if(configuration)
			BlobVFS::BlobLinker::generateLinkers(**in, **out,
					configuration->getPathPrefix(), configuration->getFileSuffix());
		else
			BlobVFS::BlobLinker::generateLinkers(**in, **out);
		outCloser.close();
		inCloser.close();
	}

	void BlobAliasGenerationTransform::wouldPerform(BuildContext& context, Artifact& target) {
		context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"would generate", getSource().getLabel(), target.getLabel()), false);
		target.getOutputStream(context, Artifact::FOR_PREDICTION, this);
		target.wouldModify(context);
	}

	void BlobAliasGenerationTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "BlobAliasGenerationTransform " << this << " {" << endln << shift;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
