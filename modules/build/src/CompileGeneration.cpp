#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "Compiler.hpp"
#include "CompileGeneration.hpp"
#include "CompilerConfiguration.hpp"

using std::list;
using redengine::util::Delete;
using redengine::util::StringUtils;
using redengine::error::IllegalArgumentError;

namespace redengine {
namespace build {

	CompileGeneration::CompileGeneration(Compiler& compiler, Compilation::CompileMode mode,
			CompilerConfiguration& configuration) : compiler(compiler), mode(mode), configuration(configuration) {}

	CompileGeneration::CompileGeneration(const CompileGeneration& generation)
			: Generation<FileArtifact>(generation), compiler(generation.compiler), mode(generation.mode),
			configuration(generation.configuration) {}

	void CompileGeneration::generate(const list<FileArtifact*>& sources, FileArtifact* target, BuildContext&) {
		if(sources.empty() || !target)
			return;
		if(sources.size() > static_cast<list<FileArtifact>::size_type>(1u))
			throw IllegalArgumentError("Compilation cannot have more than one source: "
					+ StringUtils::toString(sources.size()));
		FileArtifact* source = sources.front();
		if(!source)
			return;
		Delete<Compilation> compilation(compiler.newCompilation(source->getPathname(), mode));
		compilation->setTarget(target->getPathname());
		configuration.applyConfiguration(**compilation);
		compilation->invoke();
	}

}}
