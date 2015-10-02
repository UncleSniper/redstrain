#include <redstrain/util/Delete.hpp>

#include "Compiler.hpp"
#include "CompileTransform.hpp"
#include "CompilerConfiguration.hpp"

using std::string;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	CompileTransform::CompileTransform(Compiler& compiler, Compilation::CompileMode mode,
			CompilerConfiguration& configuration, FileArtifact& source) : OneToOneTransform<FileArtifact>(source),
			compiler(compiler), mode(mode), configuration(configuration) {}

	CompileTransform::CompileTransform(const CompileTransform& transform)
			: OneToOneTransform<FileArtifact>(transform), compiler(transform.compiler), mode(transform.mode),
			configuration(transform.configuration) {}

	struct CompileXformSourceSink : Appender<string> {

		CompileTransform& transform;
		BuildContext& context;
		Artifact& target;

		CompileXformSourceSink(CompileTransform& transform, BuildContext& context, Artifact& target)
				: transform(transform), context(context), target(target) {}

		virtual void append(const string&);

	};

	struct CompileXformTargetSink : Appender<string> {

		CompileXformSourceSink& sourceSink;
		const string& sourcePath;

		CompileXformTargetSink(CompileXformSourceSink& sourceSink, const string& sourcePath)
				: sourceSink(sourceSink), sourcePath(sourcePath) {}

		virtual void append(const string&);

	};

	void CompileTransform::perform(BuildContext& context, Artifact& target) {
		CompileXformSourceSink sink(*this, context, target);
		getSource().getFileReference("", sink, Artifact::FOR_INPUT, context);
	}

	void CompileXformSourceSink::append(const string& source) {
		CompileXformTargetSink sink(*this, source);
		target.getFileReference("", sink, Artifact::FOR_OUTPUT, context);
	}

	void CompileXformTargetSink::append(const string& targetPath) {
		Delete<Compilation> compilation(sourceSink.transform.getCompiler().newCompilation(sourcePath,
				sourceSink.transform.getCompileMode()));
		compilation->setTarget(targetPath);
		sourceSink.transform.getCompilerConfiguration().applyConfiguration(**compilation);
		compilation->invoke();
	}

	void CompileTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "CompileTransform " << this << " {" << endln << shift;
		stream << indent << "mode = " << Compilation::getCompileModeName(mode) << endln;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
