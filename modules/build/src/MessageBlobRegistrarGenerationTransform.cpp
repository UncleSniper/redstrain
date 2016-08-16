#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/locale/MessageBlobRegistrar.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "BuildContext.hpp"
#include "MessageBlobRegistrarGenerationTransform.hpp"

using std::string;
using redengine::util::Delete;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::locale::MessageBlobRegistrar;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	MessageBlobRegistrarGenerationTransform::MessageBlobRegistrarGenerationTransform(FileArtifact& source,
			MessageBlobRegisterLanguage::RegistrarConfiguration& configuration)
			: OneToOneTransform<FileArtifact>(source), configuration(configuration) {
		configuration.ref();
	}

	MessageBlobRegistrarGenerationTransform::MessageBlobRegistrarGenerationTransform(const
			MessageBlobRegistrarGenerationTransform& transform)
			: OneToOneTransform<FileArtifact>(transform), configuration(transform.configuration) {
		configuration.ref();
	}

	MessageBlobRegistrarGenerationTransform::~MessageBlobRegistrarGenerationTransform() {
		configuration.unref();
	}

	void MessageBlobRegistrarGenerationTransform::perform(BuildContext& context, Artifact& target) {
		context.getUI().willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"generating", getSource().getLabel(), target.getLabel()), false);
		Delete<OutputStream<char> > out(target.getOutputStream(context, Artifact::FOR_USE, this));
		StreamCloser outCloser(*out);
		MessageBlobRegistrar::generateBlobRegistrar(**out, configuration.getMappingSymbol(),
				configuration.getBlobSymbol(), configuration.getLanguage(), configuration.getCountry(), true, 0u);
		outCloser.close();
	}

	void MessageBlobRegistrarGenerationTransform::wouldPerform(BuildContext& context, Artifact& target) {
		context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"would generate", getSource().getLabel(), target.getLabel()), false);
		target.getOutputStream(context, Artifact::FOR_PREDICTION, this);
		target.wouldModify(context);
	}

	bool MessageBlobRegistrarGenerationTransform::onlyModifiesTarget() const {
		return true;
	}

	void MessageBlobRegistrarGenerationTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream)
			const {
		stream << indent << "MessageBlobRegistrarGenerationTransform " << this << " {" << endln << shift;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
