#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "Action.hpp"
#include "BuildUI.hpp"
#include "Compiler.hpp"
#include "CompileGeneration.hpp"
#include "CompilerConfiguration.hpp"

using std::list;
using redengine::util::Delete;
using redengine::util::StringUtils;
using redengine::error::IllegalArgumentError;
#ifdef TESTING_REDSTRAIN_BUILD_API
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;
#endif /* TESTING_REDSTRAIN_BUILD_API */

namespace redengine {
namespace build {

	CompileGeneration::CompileGeneration(Compiler& compiler, Compilation::CompileMode mode,
			CompilerConfiguration& configuration) : compiler(compiler), mode(mode), configuration(configuration) {}

	CompileGeneration::CompileGeneration(const CompileGeneration& generation)
			: Generation<FileArtifact>(generation), compiler(generation.compiler), mode(generation.mode),
			configuration(generation.configuration) {}

	FileArtifact* CompileGeneration::getSoleSource(const list<FileArtifact*>& sources) {
		if(sources.empty())
			return NULL;
		if(sources.size() > static_cast<list<FileArtifact*>::size_type>(1u))
			throw IllegalArgumentError("Compilation cannot have more than one source: "
					+ StringUtils::toString(sources.size()));
		return sources.front();
	}

	void CompileGeneration::generate(const list<FileArtifact*>& sources, FileArtifact* target, BuildContext&) {
		FileArtifact* source = CompileGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		Delete<Compilation> compilation(compiler.newCompilation(source->getPathname(), mode));
		compilation->setTarget(target->getPathname());
		configuration.applyConfiguration(**compilation);
		compilation->invoke();
	}

	void CompileGeneration::notifyUIWillGenerate(BuildUI& ui, const Action& action,
			const list<FileArtifact*>& sources, FileArtifact* target) {
		FileArtifact* source = CompileGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		ui.willPerformAction(BuildUI::ActionDescriptor(action.getComponentType(), action.getComponentName(),
				"compiling", source->getHumanReadableReference(false), target->getHumanReadableReference(true)));
	}

	void CompileGeneration::notifyUIWouldGenerate(BuildUI& ui, const Action& action,
			const list<FileArtifact*>& sources, FileArtifact* target) {
		FileArtifact* source = CompileGeneration::getSoleSource(sources);
		if(!source || !target)
			return;
		ui.wouldPerformAction(BuildUI::ActionDescriptor(action.getComponentType(), action.getComponentName(),
				"would compile", source->getHumanReadableReference(false), target->getHumanReadableReference(true)));
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void CompileGeneration::dumpGeneration(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "CompileGeneration {" << endln << shift;
		stream << indent << "mode = ";
		switch(mode) {
			#define clamp(name) \
				case Compilation::name: \
					stream << #name; \
					break;
			clamp(FOR_STATIC_EXECUTABLE)
			clamp(FOR_DYNAMIC_EXECUTABLE)
			clamp(FOR_STATIC_LIBRARY)
			clamp(FOR_DYNAMIC_LIBRARY)
			#undef clamp
			default:
				stream << "<unknown>";
				break;
		}
		stream << endln;
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}
