#include <redstrain/util/Delete.hpp>
#include <redstrain/build/Project.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/build/BuildContext.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/build/ProjectBuilder.hpp>
#include <redstrain/build/ConsoleBuildUI.hpp>
#include <redstrain/build/boot/XakeProject.hpp>
#include <redstrain/build/ComponentRuleBuilder.hpp>
#include <redstrain/build/ComponentGoalBuilder.hpp>
#include <redstrain/build/boot/XakeProjectFactory.hpp>
#include <redstrain/build/boot/XakeComponentFinder.hpp>
#include <redstrain/build/boot/XakeComponentFactory.hpp>
#include <redstrain/error/UnsupportedOperationError.hpp>
#include <redstrain/build/DefaultGoalPropertyInjector.hpp>
#include <redstrain/build/boot/XakeDependencyResolver.hpp>
#include <redstrain/build/boot/XakeBuildArtifactMapper.hpp>
#include <redstrain/build/boot/XakeArtifactStageMapper.hpp>
#include <redstrain/build/boot/XakeBuildDirectoryMapper.hpp>
#include <redstrain/build/DefaultTransformPropertyInjector.hpp>
#include <redstrain/build/boot/XakeComponentTypeStringifier.hpp>
#include <redstrain/cmdline/parseopt.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Options.hpp"

using std::string;
using redengine::util::Delete;
using redengine::build::Component;
using redengine::platform::Console;
using redengine::platform::Pathname;
using redengine::build::BuildContext;
using redengine::cmdline::OptionLogic;
using redengine::io::FileOutputStream;
using redengine::build::ProjectBuilder;
using redengine::build::ConsoleBuildUI;
using redengine::build::boot::XakeProject;
using redengine::build::ComponentRuleBuilder;
using redengine::build::ComponentGoalBuilder;
using redengine::build::boot::XakeProjectFactory;
using redengine::build::boot::XakeComponentFinder;
using redengine::error::UnsupportedOperationError;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::build::boot::XakeComponentFactory;
using redengine::build::DefaultGoalPropertyInjector;
using redengine::build::boot::XakeDependencyResolver;
using redengine::build::boot::XakeBuildArtifactMapper;
using redengine::build::boot::XakeArtifactStageMapper;
using redengine::build::boot::XakeBuildDirectoryMapper;
using redengine::build::DefaultTransformPropertyInjector;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::build::boot::XakeComponentTypeStringifier;
using redengine::io::operator<<;
using redengine::io::config::tabulation;
using redengine::cmdline::runWithOptions;

int run(const string&, const Options&);
int bootstrap(const string&, const Options&);
int normal(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("bootstrap", &Options::setBootstrap);
	logic.addLongOption("dry", &Options::setDry);
	logic.addShortOption('y', &Options::setDry);
	logic.addLongOption("base", &Options::setBase, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("dump-context", &Options::setDumpContext);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

int run(const string& progname, const Options& options) {
	if(options.isBootstrap())
		return bootstrap(progname, options);
	else
		return normal(progname, options);
}

int bootstrap(const string&, const Options& options) {
	string base(Pathname::tidy(Pathname::join(Pathname::getWorkingDirectory(), options.getBase())));
	XakeProject xproject(base);
	XakeProjectFactory projectFactory(xproject);
	XakeComponentFinder componentFinder(xproject);
	XakeComponentFactory componentFactory(xproject);
	XakeDependencyResolver dependencyResolver;
	XakeBuildDirectoryMapper directoryMapper(xproject);
	XakeBuildArtifactMapper artifactMapper(xproject);
	XakeComponentTypeStringifier componentTypeStringifier(xproject);
	DefaultTransformPropertyInjector transformPropertyInjector(&componentTypeStringifier);
	XakeArtifactStageMapper artifactStageMapper;
	DefaultGoalPropertyInjector goalPropertyInjector(&componentTypeStringifier);
	ComponentRuleBuilder ruleBuilder(directoryMapper, artifactMapper,
			&transformPropertyInjector, &artifactStageMapper, &goalPropertyInjector);
	ComponentGoalBuilder goalBuilder;
	ProjectBuilder projectBuilder(projectFactory, componentFinder, componentFactory,
			dependencyResolver, ruleBuilder, goalBuilder);
	projectBuilder.buildProject(base);
	ConsoleBuildUI ui;
	ui.setMinimalComponentTypeWidth(static_cast<unsigned>(
			Component::getMaximalComponentTypeWidth(componentTypeStringifier)));
	ui.setMinimalComponentNameWidth(static_cast<unsigned>(
			projectBuilder.getProject()->getMaximalComponentNameWidth()));
	Delete<BuildContext> context(projectBuilder.newBuildContext(ui));
	//TODO: gather requested goals
	if(options.isDumpContext()) {
		FileOutputStream sout(Console::getStandardHandle(Console::STANDARD_OUTPUT));
		DefaultConfiguredOutputStream<char>::Stream formatted(sout);
		formatted << tabulation<char>("    ");
		context->dumpContext(formatted);
		return 0;
	}
	//TODO: perform run
	return 0;
}

int normal(const string&, const Options&) {
	throw UnsupportedOperationError("Normal mode is not supported yet");
}
