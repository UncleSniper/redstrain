#include <redstrain/platform/Pathname.hpp>
#include <redstrain/build/ProjectBuilder.hpp>
#include <redstrain/build/boot/XakeProject.hpp>
#include <redstrain/build/ComponentRuleBuilder.hpp>
#include <redstrain/build/ComponentGoalBuilder.hpp>
#include <redstrain/build/boot/XakeProjectFactory.hpp>
#include <redstrain/build/boot/XakeComponentFinder.hpp>
#include <redstrain/build/boot/XakeComponentFactory.hpp>
#include <redstrain/error/UnsupportedOperationError.hpp>
#include <redstrain/build/boot/XakeDependencyResolver.hpp>
#include <redstrain/build/boot/XakeBuildArtifactMapper.hpp>
#include <redstrain/build/boot/XakeBuildDirectoryMapper.hpp>
#include <redstrain/build/DefaultTransformPropertyInjector.hpp>
#include <redstrain/build/boot/XakeComponentTypeStringifier.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"

using std::string;
using redengine::platform::Pathname;
using redengine::cmdline::OptionLogic;
using redengine::build::ProjectBuilder;
using redengine::build::boot::XakeProject;
using redengine::build::ComponentRuleBuilder;
using redengine::build::ComponentGoalBuilder;
using redengine::build::boot::XakeProjectFactory;
using redengine::build::boot::XakeComponentFinder;
using redengine::error::UnsupportedOperationError;
using redengine::build::boot::XakeComponentFactory;
using redengine::build::boot::XakeDependencyResolver;
using redengine::build::boot::XakeBuildArtifactMapper;
using redengine::build::boot::XakeBuildDirectoryMapper;
using redengine::build::DefaultTransformPropertyInjector;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::build::boot::XakeComponentTypeStringifier;
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
	ComponentRuleBuilder ruleBuilder(directoryMapper, artifactMapper, &transformPropertyInjector);
	ComponentGoalBuilder goalBuilder;
	ProjectBuilder projectBuilder(projectFactory, componentFinder, componentFactory,
			dependencyResolver, ruleBuilder, goalBuilder);
	projectBuilder.buildProject(base);
	//TODO: create build context...
	return 0;
}

int normal(const string&, const Options&) {
	throw UnsupportedOperationError("Normal mode is not supported yet");
}
