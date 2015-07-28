#include <redstrain/util/Delete.hpp>
#include <redstrain/build/Project.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/build/BuildContext.hpp>
#include <redstrain/build/ConsoleBuildUI.hpp>
#include <redstrain/build/boot/XakeProject.hpp>
#include <redstrain/build/NoSuchValveError.hpp>
#include <redstrain/build/ComponentRuleBuilder.hpp>
#include <redstrain/build/boot/XakeValveInjector.hpp>
#include <redstrain/build/boot/XakeProjectFactory.hpp>
#include <redstrain/build/boot/XakeComponentFinder.hpp>
#include <redstrain/build/boot/XakeComponentFactory.hpp>
#include <redstrain/error/UnsupportedOperationError.hpp>
#include <redstrain/build/boot/XakeDependencyResolver.hpp>
#include <redstrain/build/boot/XakeBuildArtifactMapper.hpp>
#include <redstrain/build/boot/XakeBuildDirectoryMapper.hpp>
#include <redstrain/build/DefaultComponentTypeStringifier.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"

using std::string;
using redengine::util::Delete;
using redengine::build::Component;
using redengine::platform::Pathname;
using redengine::build::StaticValve;
using redengine::build::BuildContext;
using redengine::cmdline::OptionLogic;
using redengine::build::ConsoleBuildUI;
using redengine::build::ProjectBuilder;
using redengine::build::NoSuchValveError;
using redengine::cmdline::runWithOptions;
using redengine::build::boot::XakeProject;
using redengine::build::ComponentRuleBuilder;
using redengine::build::boot::XakeValveInjector;
using redengine::build::boot::XakeProjectFactory;
using redengine::build::boot::XakeComponentFinder;
using redengine::error::UnsupportedOperationError;
using redengine::build::boot::XakeComponentFactory;
using redengine::build::boot::XakeDependencyResolver;
using redengine::build::boot::XakeBuildArtifactMapper;
using redengine::build::boot::XakeBuildDirectoryMapper;
using redengine::build::DefaultComponentTypeStringifier;
using redengine::cmdline::ConfigurationObjectOptionLogic;

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
	XakeBuildArtifactMapper buildArtifactMapper(xproject);
	XakeValveInjector valveInjector(xproject);
	ComponentRuleBuilder ruleBuilder(buildDirectoryMapper, buildArtifactMapper,
			DefaultComponentTypeStringifier::instance, &valveInjector);
	ProjectBuilder projectBuilder(projectFactory, componentFinder, componentFactory,
			dependencyResolver, ruleBuilder);
	projectBuilder.buildProject(base);
	ConsoleBuildUI ui;
	Delete<BuildContext> context(projectBuilder.newBuildContext(ui));
	xproject.makeValveGroups(**context);
	ui.setMinimalComponentTypeWidth(static_cast<unsigned>(
			Component::getMaximalComponentTypeWidth(DefaultComponentTypeStringifier::instance)));
	ui.setMinimalComponentNameWidth(static_cast<unsigned>(
			projectBuilder.getProject()->getMaximalComponentNameWidth()));
	Options::ValveNameIterator ovbegin, ovend;
	options.getValves(ovbegin, ovend);
	for(; ovbegin != ovend; ++ovbegin) {
		StaticValve* valve = context->getValve(*ovbegin);
		if(!valve)
			throw NoSuchValveError(*ovbegin);
		valve->setOpen(true);
	}
	context->forceValveGroups();
	if(options.isDry())
		context->predictiveLoop();
	else {
		ui.setFlags(ConsoleBuildUI::PRINT_DEFINITIVE);
		context->predictiveLoop();
		context->definitiveLoop();
	}
	return 0;
}

int normal(const string&, const Options&) {
	throw UnsupportedOperationError("Normal mode is not supported yet");
}
