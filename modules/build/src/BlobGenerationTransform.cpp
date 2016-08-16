#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "BuildContext.hpp"
#include "BlobGenerationTransform.hpp"

using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::io::CPPArrayOutputStream;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	BlobGenerationTransform::BlobGenerationTransform(FileArtifact& source,
			BlobLanguage::BlobConfiguration* configuration, bool header) : OneToOneTransform<FileArtifact>(source),
			configuration(configuration), header(header) {
		if(configuration)
			configuration->ref();
	}

	BlobGenerationTransform::BlobGenerationTransform(const BlobGenerationTransform& transform)
			: OneToOneTransform<FileArtifact>(transform), configuration(transform.configuration),
			header(transform.header) {
		if(configuration)
			configuration->ref();
	}

	BlobGenerationTransform::~BlobGenerationTransform() {
		if(configuration)
			configuration->unref();
	}

	void BlobGenerationTransform::perform(BuildContext& context, Artifact& target) {
		context.getUI().willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"generating", getSource().getLabel(), target.getLabel()), false);
		Delete<InputStream<char> > in;
		if(!header)
			in = getSource().getInputStream(context);
		StreamCloser inCloser(*in);
		Delete<OutputStream<char> > out(target.getOutputStream(context, Artifact::FOR_USE, this));
		StreamCloser outCloser(*out);
		CPPArrayOutputStream gen(**out);
		if(configuration)
			configuration->applyConfiguration(gen);
		if(header)
			gen.writeHeader();
		else
			in->copyTo(gen);
		gen.close();
		outCloser.release();
		inCloser.close();
	}

	void BlobGenerationTransform::wouldPerform(BuildContext& context, Artifact& target) {
		context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"would generate", getSource().getLabel(), target.getLabel()), false);
		target.getOutputStream(context, Artifact::FOR_PREDICTION, this);
		target.wouldModify(context);
	}

	bool BlobGenerationTransform::onlyModifiesTarget() const {
		return true;
	}

	void BlobGenerationTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "BlobGenerationTransform " << this << " {" << endln << shift;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
