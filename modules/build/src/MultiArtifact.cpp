#include <sstream>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "MultiArtifact.hpp"
#include "ArtifactMultiplicityError.hpp"

using std::string;
using std::stringstream;
using redengine::util::Appender;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::util::MemorySize;
using redengine::platform::Pathname;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	MultiArtifact::MultiArtifact(const string& label) : label(label) {}

	MultiArtifact::MultiArtifact(const MultiArtifact& artifact)
			: Artifact(artifact), artifacts(artifact.artifacts), label(artifact.label) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	MultiArtifact::~MultiArtifact() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	Artifact& MultiArtifact::checkSingleChild(const char* action) {
		if(artifacts.size() == static_cast<MemorySize>(1u))
			return **artifacts.begin();
		throw ArtifactMultiplicityError(*this, "Cannot " + string(action) + " for composite artifact");
	}

	bool MultiArtifact::addArtifact(Artifact& artifact) {
		if(!artifacts.append(&artifact))
			return false;
		artifact.ref();
		return true;
	}

	bool MultiArtifact::removeArtifact(Artifact& artifact) {
		if(!artifacts.erase(&artifact))
			return false;
		artifact.unref();
		return true;
	}

	void MultiArtifact::clearArtifacts() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	void MultiArtifact::getArtifacts(ArtifactIterator& begin, ArtifactIterator& end) const {
		begin = artifacts.begin();
		end = artifacts.end();
	}

	void MultiArtifact::setLabel(const string& label) {
		this->label = label;
	}

	bool MultiArtifact::wouldBePresent() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin) {
			if(!(*begin)->wouldBePresent())
				return false;
		}
		return true;
	}

	void MultiArtifact::getPredictedModificationTimestamp(Appender<time_t>& sink) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->getPredictedModificationTimestamp(sink);
	}

	void MultiArtifact::getPredictedVirtualClock(Appender<time_t>& sink) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->getPredictedVirtualClock(sink);
	}

	void MultiArtifact::wouldModify(BuildContext& context) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->wouldModify(context);
	}

	void MultiArtifact::wouldRemove() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->wouldRemove();
	}

	bool MultiArtifact::isPresent() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin) {
			if(!(*begin)->isPresent())
				return false;
		}
		return true;
	}

	void MultiArtifact::getModificationTimestamp(Appender<time_t>& sink) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->getModificationTimestamp(sink);
	}

	void MultiArtifact::getVirtualClock(Appender<time_t>& sink) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->getVirtualClock(sink);
	}

	void MultiArtifact::notifyModified(BuildContext& context) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->notifyModified(context);
	}

	void MultiArtifact::remove() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->remove();
	}

	InputStream<char>* MultiArtifact::getInputStream(BuildContext& context) {
		return checkSingleChild("obtain input stream").getInputStream(context);
	}

	OutputStream<char>* MultiArtifact::getOutputStream(BuildContext& context, ReferenceMood mood,
			const ComponentUIInfo* component) {
		return checkSingleChild("obtain output stream").getOutputStream(context, mood, component);
	}

	void MultiArtifact::getFileReference(const string& suffix, Appender<string>& sink,
			ReferenceDirection direction, ReferenceMood mood, BuildContext& context,
			const ComponentUIInfo* component) {
		if(!suffix.empty())
			checkSingleChild("obtain file reference with non-empty suffix");
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->getFileReference(suffix, sink, direction, mood, context, component);
	}

	bool MultiArtifact::isTransformable() {
		return false;
	}

	string MultiArtifact::getName() {
		return "";
	}

	string MultiArtifact::getLabel() {
		return label;
	}

	string MultiArtifact::getCanonicalReference() {
		stringstream ss;
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		bool first = true;
		for(; begin != end; ++begin) {
			if(first)
				first = false;
			else
				ss << Pathname::PATHNAME_SEPARATOR;
			ss << (*begin)->getCanonicalReference();
		}
		return ss.str();
	}

	void MultiArtifact::dumpArtifact(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "MultiArtifact " << this << " {" << endln << shift;
		stream << indent << "label = '" << label << '\'' << endln;
		stream << indent << "artifacts = {" << endln << shift;
		ArtifactIterator abegin(artifacts.begin()), aend(artifacts.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		dumpArtifactAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
