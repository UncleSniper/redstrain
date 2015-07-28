#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/text/CachedCodeTable.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>
#include <redstrain/text/CodeTableDefinitionReader.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Action.hpp"
#include "BuildUI.hpp"
#include "CodeTableCompileGeneration.hpp"

using std::list;
using redengine::text::Char16;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::text::CachedCodeTable;
using redengine::error::IllegalArgumentError;
using redengine::text::CodeTableDefinitionReader;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	CodeTableCompileGeneration::CodeTableCompileGeneration() {}

	CodeTableCompileGeneration::CodeTableCompileGeneration(const CodeTableCompileGeneration& generation)
			: Generation<FileArtifact>(generation) {}

	FileArtifact* CodeTableCompileGeneration::getSoleSource(const list<FileArtifact*>& sources) {
		if(sources.empty())
			return NULL;
		if(sources.size() > static_cast<list<FileArtifact*>::size_type>(1u))
			throw IllegalArgumentError("Code table compilation cannot have more than one source: "
					+ StringUtils::toString(sources.size()));
		return sources.front();
	}

	void CodeTableCompileGeneration::generate(const list<FileArtifact*>& sources, FileArtifact* target,
			BuildContext&) {
		FileArtifact* source = CodeTableCompileGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		FileInputStream in(source->getPathname());
		StreamCloser inCloser(in);
		CachedCodeTable<Char16> table;
		CodeTableDefinitionReader<Char16, char>::CachedCodeTableSink sink(table);
		CodeTableDefinitionReader<Char16, char>::readDefinition(in, sink);
		inCloser.close();
		FileOutputStream out(target->getPathname());
		StreamCloser outCloser(out);
		table.saveTo(out);
		outCloser.close();
	}

	void CodeTableCompileGeneration::notifyUIWillGenerate(BuildUI& ui, const Action& action,
			const list<FileArtifact*>& sources, FileArtifact* target) {
		FileArtifact* source = CodeTableCompileGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		ui.willPerformAction(BuildUI::ActionDescriptor(action.getComponentType(), action.getComponentName(),
				"converting", source->getBasename(), Pathname::stripPrefix(target->getPathname(),
				action.getComponentBaseDirectory())));
	}

	void CodeTableCompileGeneration::notifyUIWouldGenerate(BuildUI& ui, const Action& action,
			const list<FileArtifact*>& sources, FileArtifact* target) {
		FileArtifact* source = CodeTableCompileGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		ui.wouldPerformAction(BuildUI::ActionDescriptor(action.getComponentType(), action.getComponentName(),
				"would convert", source->getBasename(), Pathname::stripPrefix(target->getPathname(),
				action.getComponentBaseDirectory())));
	}

	void CodeTableCompileGeneration::dumpGeneration(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "CodeTableCompileGeneration" << endln;
	}

}}
