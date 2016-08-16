#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/locale/MessageBlobRegistrar.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "BuildContext.hpp"
#include "MessageBlobAliasGenerationTransform.hpp"

using std::string;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::platform::Pathname;
using redengine::locale::MessageBlobRegistrar;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	MessageBlobAliasGenerationTransform::MessageBlobAliasGenerationTransform(FileArtifact& source,
			MessageBlobAliasLanguage::AliasConfiguration& configuration)
			: OneToOneTransform<FileArtifact>(source), configuration(configuration) {
		configuration.ref();
	}

	MessageBlobAliasGenerationTransform::MessageBlobAliasGenerationTransform(const
			MessageBlobAliasGenerationTransform& transform)
			: OneToOneTransform<FileArtifact>(transform), configuration(transform.configuration) {
		configuration.ref();
	}

	MessageBlobAliasGenerationTransform::~MessageBlobAliasGenerationTransform() {
		configuration.unref();
	}

	void MessageBlobAliasGenerationTransform::perform(BuildContext& context, Artifact& target) {
		context.getUI().willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"generating", getSource().getLabel(), target.getLabel()), false);
		Delete<InputStream<char> > in(getSource().getInputStream(context));
		StreamCloser inCloser(*in);
		Delete<OutputStream<char> > out(target.getOutputStream(context, Artifact::FOR_USE, this));
		StreamCloser outCloser(*out);
		string path(getSource().getPath());
		MessageBlobRegistrar::FileIncludeResolver includeResolver(Pathname::dirname(path, Pathname::LOGICAL));
		bool withInclude = true;
		unsigned nextID = 0u;
		MessageBlobRegistrar::generateBlobAliases(**in, path, includeResolver, **out, withInclude,
				configuration.getMappingSymbol(), configuration.getBlobNamespacePrefix(), nextID);
		outCloser.close();
		inCloser.close();
	}

	void MessageBlobAliasGenerationTransform::wouldPerform(BuildContext& context, Artifact& target) {
		context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"would generate", getSource().getLabel(), target.getLabel()), false);
		target.getOutputStream(context, Artifact::FOR_PREDICTION, this);
		target.wouldModify(context);
	}

	bool MessageBlobAliasGenerationTransform::onlyModifiesTarget() const {
		return true;
	}

	void MessageBlobAliasGenerationTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream)
			const {
		stream << indent << "MessageBlobAliasGenerationTransform " << this << " {" << endln << shift;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
