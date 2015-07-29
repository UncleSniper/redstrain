#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "BlobGeneration.hpp"

using std::list;
using redengine::util::Delete;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::io::CPPArrayOutputStream;
using redengine::error::IllegalArgumentError;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	BlobGeneration::BlobGeneration(BlobLanguage::BlobConfiguration* configuration, bool header)
			: configuration(configuration), header(header) {
		if(configuration)
			configuration->ref();
	}

	BlobGeneration::BlobGeneration(const BlobGeneration& generation)
			: Generation<FileArtifact>(generation), configuration(generation.configuration),
			header(generation.header) {
		if(configuration)
			configuration->ref();
	}

	BlobGeneration::~BlobGeneration() {
		if(configuration)
			configuration->unref();
	}

	FileArtifact* BlobGeneration::getSoleSource(const list<FileArtifact*>& sources) {
		if(sources.empty())
			return NULL;
		if(sources.size() > static_cast<list<FileArtifact*>::size_type>(1u))
			throw IllegalArgumentError("Blob cannot have more than one source: "
					+ StringUtils::toString(sources.size()));
		return sources.front();
	}

	void BlobGeneration::generate(const list<FileArtifact*>& sources, FileArtifact* target, BuildContext&) {
		FileArtifact* source = BlobGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		Delete<FileInputStream> in;
		if(!header)
			in = new FileInputStream(source->getPathname());
		StreamCloser inCloser(*in);
		FileOutputStream out(target->getPathname());
		StreamCloser outCloser(out);
		CPPArrayOutputStream gen(out);
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

	void BlobGeneration::notifyUIWillGenerate(BuildUI& ui, const Action& action,
			const list<FileArtifact*>& sources, FileArtifact* target) {
		FileArtifact* source = BlobGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		ui.willPerformAction(BuildUI::ActionDescriptor(action.getComponentType(), action.getComponentName(),
				"generating", source->getBasename(), Pathname::stripPrefix(target->getPathname(),
				action.getComponentBaseDirectory())));
	}

	void BlobGeneration::notifyUIWouldGenerate(BuildUI& ui, const Action& action,
			const list<FileArtifact*>& sources, FileArtifact* target) {
		FileArtifact* source = BlobGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		ui.wouldPerformAction(BuildUI::ActionDescriptor(action.getComponentType(), action.getComponentName(),
				"would generate", source->getBasename(), Pathname::stripPrefix(target->getPathname(),
				action.getComponentBaseDirectory())));
	}

	void BlobGeneration::dumpGeneration(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "BlobGeneration" << endln;
	}

}}
