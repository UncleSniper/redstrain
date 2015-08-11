#include <redstrain/util/Unref.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "Compiler.hpp"
#include "BuildContext.hpp"
#include "CopyTransform.hpp"
#include "CompileTransform.hpp"
#include "CompiledLanguage.hpp"
#include "EmptyCompilerConfiguration.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Appender;
using redengine::platform::Pathname;

namespace redengine {
namespace build {

	CompiledLanguage::CompiledLanguage(const string& name, Compiler& compiler)
			: Language(name), compiler(compiler) {}

	CompiledLanguage::CompiledLanguage(const CompiledLanguage& language)
			: Language(language), compiler(language.compiler) {}

	CompilerConfiguration& CompiledLanguage::getCompilerConfiguration(const Flavor&, const Component&) {
		return EmptyCompilerConfiguration::instance;
	}

	bool CompiledLanguage::acceptsSource(const Flavor&, const Flavor&) {
		return true;
	}

	static const Flavor *const FLAVORS[] = {
		&Flavor::STATIC,
		&Flavor::DYNAMIC,
		NULL
	};

	void CompiledLanguage::getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		Language::sinkSupportedFlavors(FLAVORS, sink);
	}

	bool CompiledLanguage::isOneToOne(const Flavor&) {
		return true;
	}

	FileArtifact* CompiledLanguage::getSourceTransform(BuildContext& context, const string& sourceDirectory,
			FileArtifact& sourceArtifact, const Flavor&, const string& targetDirectory,
			const Flavor& transformFlavor, Component& component, BuildArtifactMapper&,
			ManyToOneTransform<FileArtifact>*& manyTransform, Flavor& targetFlavor, bool& isFinal) {
		Compilation::CompileMode mode;
		if(transformFlavor == Flavor::STATIC)
			mode = component.getType() == Component::EXECUTABLE
					? Compilation::FOR_STATIC_EXECUTABLE : Compilation::FOR_STATIC_LIBRARY;
		else if(transformFlavor == Flavor::DYNAMIC)
			mode = component.getType() == Component::EXECUTABLE
					? Compilation::FOR_DYNAMIC_EXECUTABLE : Compilation::FOR_DYNAMIC_LIBRARY;
		else
			return NULL;
		string sourceBasename(Pathname::stripPrefix(sourceArtifact.getPath(), sourceDirectory));
		string targetBasename(compiler.getObjectFileNameForSource(sourceBasename));
		FileArtifact& targetArtifact = context.internFileArtifact(Pathname::tidy(Pathname::join(targetDirectory,
				targetBasename)), targetBasename);
		Unref<CompileTransform> transform(new CompileTransform(compiler, mode,
				getCompilerConfiguration(transformFlavor, component), sourceArtifact));
		transform->addPrerequisite(sourceArtifact);
		targetArtifact.setGeneratingTransform(*transform);
		manyTransform = NULL;
		targetFlavor = transformFlavor;
		isFinal = false;
		targetArtifact.ref();
		return &targetArtifact;
	}

	FileArtifact* CompiledLanguage::getHeaderExposeTransform(BuildContext& context, const string& sourceDirectory,
			FileArtifact& sourceArtifact, const Flavor&, const string& targetDirectory, Component&,
			BuildArtifactMapper&, Flavor& targetFlavor) {
		string sourceBasename(Pathname::stripPrefix(sourceArtifact.getPath(), sourceDirectory));
		FileArtifact& targetArtifact = context.internFileArtifact(Pathname::tidy(Pathname::join(targetDirectory,
				sourceBasename)), sourceBasename);
		Unref<CopyTransform> transform(new CopyTransform(sourceArtifact));
		transform->addPrerequisite(sourceArtifact);
		targetArtifact.setGeneratingTransform(*transform);
		targetFlavor = Flavor::HEADER;
		targetArtifact.ref();
		return &targetArtifact;
	}

}}
