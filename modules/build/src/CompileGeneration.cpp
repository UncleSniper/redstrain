#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

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
