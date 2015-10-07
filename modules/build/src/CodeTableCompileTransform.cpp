#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/text/CodeTableDefinitionReader.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "BuildContext.hpp"
#include "CodeTableCompileTransform.hpp"

using redengine::text::Char16;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::text::CachedCodeTable;
using redengine::text::CodeTableDefinitionReader;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	CodeTableCompileTransform::CodeTableCompileTransform(FileArtifact& source)
			: OneToOneTransform<FileArtifact>(source) {}

	CodeTableCompileTransform::CodeTableCompileTransform(const CodeTableCompileTransform& transform)
			: OneToOneTransform<FileArtifact>(transform) {}

	void CodeTableCompileTransform::perform(BuildContext& context, Artifact& target) {
		context.getUI().willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"generating", getSource().getLabel(), target.getLabel()), false);
		Delete<InputStream<char> > in(getSource().getInputStream(context));
		StreamCloser inCloser(*in);
		CachedCodeTable<Char16> table;
		CodeTableDefinitionReader<Char16, char>::CachedCodeTableSink sink(table);
		CodeTableDefinitionReader<Char16, char>::readDefinition(**in, sink);
		inCloser.close();
		Delete<OutputStream<char> > out(target.getOutputStream(context, Artifact::FOR_USE, this));
		StreamCloser outCloser(*out);
		table.saveTo(**out);
		outCloser.close();
	}

	void CodeTableCompileTransform::wouldPerform(BuildContext& context, Artifact& target) {
		context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"would generate", getSource().getLabel(), target.getLabel()), false);
		target.getOutputStream(context, Artifact::FOR_PREDICTION, this);
		target.wouldModify(context);
	}

	void CodeTableCompileTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "CodeTableCompileTransform " << this << " {" << endln << shift;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
