#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/TextCodecInputStream.hpp>
#include <redstrain/locale/messages.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "BuildContext.hpp"
#include "Messages16CompileTransform.hpp"

using std::list;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::text::UTF8Decoder16;
using redengine::locale::MessageCache;
using redengine::text::Decoder16InputStream;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;
using redengine::locale::readMessages;
using redengine::locale::getMessageKeyOrder;

namespace redengine {
namespace build {

	Messages16CompileTransform::Messages16CompileTransform(FileArtifact& source)
			: OneToOneTransform<FileArtifact>(source), specification(NULL) {}

	Messages16CompileTransform::Messages16CompileTransform(const Messages16CompileTransform& transform)
			: OneToOneTransform<FileArtifact>(transform), specification(transform.specification) {
		if(specification)
			specification->ref();
	}

	Messages16CompileTransform::~Messages16CompileTransform() {
		if(specification)
			specification->unref();
	}

	void Messages16CompileTransform::setSpecificationArtifact(FileArtifact* artifact) {
		if(artifact)
			artifact->ref();
		if(specification)
			specification->unref();
		specification = artifact;
	}

	void Messages16CompileTransform::perform(BuildContext& context, Artifact& target) {
		context.getUI().willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"generating", getSource().getLabel(), target.getLabel()), false);
		// read specification
		Delete<list<String16> > order;
		if(specification) {
			Delete<InputStream<char> > specIn(specification->getInputStream(context));
			StreamCloser specInCloser(*specIn);
			UTF8Decoder16 specDecoder;
			Decoder16InputStream decSpecIn(**specIn, specDecoder);
			MessageCache<Char16> specCache;
			readMessages<Char16>(specification->getLabel(), decSpecIn, specCache, NULL);
			specInCloser.close();
			order = new list<String16>;
			getMessageKeyOrder<Char16>(specCache, NULL, *order);
		}
		// read messages
		Delete<InputStream<char> > in(getSource().getInputStream(context));
		StreamCloser inCloser(*in);
		UTF8Decoder16 decoder;
		Decoder16InputStream decIn(**in, decoder);
		MessageCache<Char16> cache;
		readMessages<Char16>(getSource().getLabel(), decIn, cache, *order);
		inCloser.close();
		// write messages
		Delete<OutputStream<char> > out(target.getOutputStream(context, Artifact::FOR_USE, this));
		StreamCloser outCloser(*out);
		cache.saveTo(**out);
		outCloser.close();
	}

	void Messages16CompileTransform::wouldPerform(BuildContext& context, Artifact& target) {
		context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
				"would generate", getSource().getLabel(), target.getLabel()), false);
		target.getOutputStream(context, Artifact::FOR_PREDICTION, this);
		target.wouldModify(context);
	}

	bool Messages16CompileTransform::onlyModifiesTarget() const {
		return true;
	}

	void Messages16CompileTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "Messages16CompileTransform " << this << " {" << endln << shift;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
