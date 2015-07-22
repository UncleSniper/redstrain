#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/build/BuildContext.hpp>
#include <redstrain/build/ConsoleBuildUI.hpp>
#include <redstrain/build/boot/XakeProject.hpp>
#include <redstrain/build/ComponentRuleBuilder.hpp>
#include <redstrain/build/boot/XakeValveInjector.hpp>
#include <redstrain/build/boot/XakeProjectFactory.hpp>
#include <redstrain/build/boot/XakeComponentFinder.hpp>
#include <redstrain/build/boot/XakeComponentFactory.hpp>
#include <redstrain/error/UnsupportedOperationError.hpp>
#include <redstrain/build/boot/XakeDependencyResolver.hpp>
#include <redstrain/build/boot/XakeBuildDirectoryMapper.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"

using std::string;
using redengine::util::Delete;
using redengine::platform::Pathname;
using redengine::build::BuildContext;
using redengine::cmdline::OptionLogic;
using redengine::build::ConsoleBuildUI;
using redengine::build::ProjectBuilder;
using redengine::cmdline::runWithOptions;
using redengine::build::boot::XakeProject;
using redengine::build::ComponentRuleBuilder;
using redengine::build::boot::XakeValveInjector;
using redengine::build::boot::XakeProjectFactory;
using redengine::build::boot::XakeComponentFinder;
using redengine::error::UnsupportedOperationError;
using redengine::build::boot::XakeComponentFactory;
using redengine::build::boot::XakeDependencyResolver;
using redengine::build::boot::XakeBuildDirectoryMapper;
using redengine::cmdline::ConfigurationObjectOptionLogic;

int run(const string&, const Options&);
int bootstrap(const string&, const Options&);
int normal(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("bootstrap", &Options::setBootstrap);
	logic.addLongOption("base", &Options::setBase, OptionLogic::REQUIRED_ARGUMENT);
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
	XakeDependencyResolver dependencyResolver(xproject);
	XakeBuildDirectoryMapper buildDirectoryMapper(xproject);
	XakeValveInjector valveInjector(xproject);
	ComponentRuleBuilder ruleBuilder(buildDirectoryMapper, &valveInjector);
	ProjectBuilder projectBuilder(projectFactory, componentFinder, componentFactory,
			dependencyResolver, ruleBuilder);
	projectBuilder.buildProject(base);
	ConsoleBuildUI ui;
	Delete<BuildContext> context(projectBuilder.newBuildContext(ui));
	xproject.makeValveGroups(**context);
	redengine::io::FileOutputStream sout(redengine::platform::Console::getStandardHandle(redengine::platform::Console::STANDARD_OUTPUT));
	redengine::io::DefaultConfiguredOutputStream<char>::Stream fout(sout);
	context->dumpContext(fout);
	return 0;
}

int normal(const string&, const Options&) {
	throw UnsupportedOperationError("Normal mode is not supported yet");
}
