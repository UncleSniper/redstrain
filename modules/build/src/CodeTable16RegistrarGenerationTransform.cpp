#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "BuildContext.hpp"
#include "CodeTable16RegistrarGenerationTransform.hpp"

using std::string;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::platform::Pathname;
using redengine::text::BlobCodeTable16Registrar;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	CodeTable16RegistrarGenerationTransform::CodeTable16RegistrarGenerationTransform(FileArtifact& source,
			CodeTable16RegisterLanguage::RegistrarConfiguration* configuration)
			: OneToOneTransform<FileArtifact>(source), configuration(configuration) {
		if(configuration)
			configuration->ref();
	}

	CodeTable16RegistrarGenerationTransform::CodeTable16RegistrarGenerationTransform(const
			CodeTable16RegistrarGenerationTransform& transform)
			: OneToOneTransform<FileArtifact>(transform), configuration(transform.configuration) {
		if(configuration)
			configuration->ref();
	}

	CodeTable16RegistrarGenerationTransform::~CodeTable16RegistrarGenerationTransform() {
		if(configuration)
			configuration->unref();
	}

	void CodeTable16RegistrarGenerationTransform::perform(BuildContext& context, Artifact& target) {
		context.getUI().willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"generating", getSource().getLabel(), target.getLabel()), false);
		// read
		string path(getSource().getPath());
		BlobCodeTable16Registrar::NameCache nameCache;
		Delete<InputStream<char> > in(getSource().getInputStream(context));
		StreamCloser inCloser(*in);
		BlobCodeTable16Registrar::GeneratorReader::FileIncludeResolver
				includeResolver(Pathname::dirname(path, Pathname::LOGICAL));
		BlobCodeTable16Registrar::GeneratorReader reader(**in, path, nameCache, includeResolver);
		if(configuration)
			configuration->applyConfiguration(reader);
		reader.readBlobNames();
		inCloser.close();
		// write
		Delete<OutputStream<char> > out(target.getOutputStream(context, Artifact::FOR_USE, this));
		StreamCloser outCloser(*out);
		BlobCodeTable16Registrar::GeneratorWriter writer(**out, NULL);
		if(configuration)
			configuration->applyConfiguration(writer);
		nameCache.generateBlobRegistrars(writer);
		outCloser.close();
	}

	void CodeTable16RegistrarGenerationTransform::wouldPerform(BuildContext& context, Artifact& target) {
		context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"would generate", getSource().getLabel(), target.getLabel()), false);
		target.getOutputStream(context, Artifact::FOR_PREDICTION, this);
		target.wouldModify(context);
	}

	void CodeTable16RegistrarGenerationTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream)
			const {
		stream << indent << "CodeTable16RegistrarGenerationTransform " << this << " {" << endln << shift;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
