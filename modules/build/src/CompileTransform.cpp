#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "BuildUI.hpp"
#include "Compiler.hpp"
#include "BuildContext.hpp"
#include "CompileTransform.hpp"
#include "CompilerConfiguration.hpp"

using std::string;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::platform::Pathname;
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
		getSource().getFileReference("", sink, Artifact::FOR_INPUT, Artifact::FOR_USE, context, this);
	}

	void CompileXformSourceSink::append(const string& source) {
		CompileXformTargetSink sink(*this, source);
		target.getFileReference("", sink, Artifact::FOR_OUTPUT, Artifact::FOR_USE, context, &transform);
	}

	void CompileXformTargetSink::append(const string& targetPath) {
		sourceSink.context.getUI().willPerformAction(BuildUI::ActionDescriptor(
				sourceSink.transform.getComponentType(), sourceSink.transform.getComponentName(),
				"compiling", Pathname::stripPrefix(sourcePath, sourceSink.transform.getComponentBaseDirectory()),
				Pathname::stripPrefix(targetPath, sourceSink.transform.getComponentBaseDirectory())), false);
		Delete<Compilation> compilation(sourceSink.transform.getCompiler().newCompilation(sourcePath,
				sourceSink.transform.getCompileMode()));
		compilation->setTarget(targetPath);
		sourceSink.transform.getCompilerConfiguration().applyConfiguration(**compilation);
		compilation->invoke();
		sourceSink.target.notifyModified(sourceSink.context);
	}

	struct PredictiveCompileXformSourceSink : Appender<string> {

		CompileTransform& transform;
		BuildContext& context;
		Artifact& target;

		PredictiveCompileXformSourceSink(CompileTransform& transform, BuildContext& context, Artifact& target)
				: transform(transform), context(context), target(target) {}

		virtual void append(const string&);

	};

	struct PredictiveCompileXformTargetSink : Appender<string> {

		PredictiveCompileXformSourceSink& sourceSink;
		const string& sourcePath;

		PredictiveCompileXformTargetSink(PredictiveCompileXformSourceSink& sourceSink, const string& sourcePath)
				: sourceSink(sourceSink), sourcePath(sourcePath) {}

		virtual void append(const string&);

	};

	void CompileTransform::wouldPerform(BuildContext& context, Artifact& target) {
		PredictiveCompileXformSourceSink sink(*this, context, target);
		getSource().getFileReference("", sink, Artifact::FOR_INPUT, Artifact::FOR_PREDICTION, context, this);
	}

	void PredictiveCompileXformSourceSink::append(const string& source) {
		PredictiveCompileXformTargetSink sink(*this, source);
		target.getFileReference("", sink, Artifact::FOR_OUTPUT, Artifact::FOR_PREDICTION, context, &transform);
	}

	void PredictiveCompileXformTargetSink::append(const string& targetPath) {
		sourceSink.context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(
				sourceSink.transform.getComponentType(), sourceSink.transform.getComponentName(),
				"would compile", Pathname::stripPrefix(sourcePath, sourceSink.transform.getComponentBaseDirectory()),
				Pathname::stripPrefix(targetPath, sourceSink.transform.getComponentBaseDirectory())), false);
		sourceSink.target.wouldModify(sourceSink.context);
	}

	bool CompileTransform::onlyModifiesTarget() const {
		return true;
	}

	void CompileTransform::dumpTransform(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "CompileTransform " << this << " {" << endln << shift;
		stream << indent << "mode = " << Compilation::getCompileModeName(mode) << endln;
		dumpOneToOneTransformAspects(stream);
		dumpTransformAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
