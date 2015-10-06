#include <redstrain/io/streamoperators.hpp>

#include "ConsoleBuildUI.hpp"

using std::string;
using redengine::platform::File;
using redengine::platform::Console;
using redengine::io::FileOutputStream;
using redengine::io::endln;
using redengine::io::operator<<;
using redengine::io::config::pad;

namespace redengine {
namespace build {

	ConsoleBuildUI::ConsoleBuildUI()
			: output(Console::getStandardHandle(Console::STANDARD_OUTPUT)), formatted(output),
			componentTypeWidth(0u), componentNameWidth(0u), flags(DEFAULT_FLAGS), console(NULL),
			deleteConsole(false) {}

	ConsoleBuildUI::ConsoleBuildUI(const File::Handle& handle)
			: output(handle), formatted(output), componentTypeWidth(0u), componentNameWidth(0u),
			flags(DEFAULT_FLAGS), console(NULL), deleteConsole(false) {}

	ConsoleBuildUI::ConsoleBuildUI(Console::StandardHandle handle)
			: output(Console::getStandardHandle(handle)), formatted(output), componentTypeWidth(0u),
			componentNameWidth(0u), flags(DEFAULT_FLAGS), console(NULL), deleteConsole(false) {}

	ConsoleBuildUI::ConsoleBuildUI(const ConsoleBuildUI& ui) : AbstractBuildUI(ui),
			output(ui.output.getFile().getHandle()), formatted(output), componentTypeWidth(ui.componentTypeWidth),
			componentNameWidth(ui.componentNameWidth), flags(ui.flags),
			console(ui.deleteConsole ? NULL : ui.console), deleteConsole(false) {}

	ConsoleBuildUI::~ConsoleBuildUI() {
		if(console && deleteConsole)
			delete console;
	}

	void ConsoleBuildUI::detectConsole() {
		if(console)
			return;
		if(Console::isConsole(output.getFile().getHandle())) {
			console = new Console(output.getFile());
			deleteConsole = true;
		}
	}

	void ConsoleBuildUI::indent(unsigned level) {
		for(; level; --level)
			formatted << ' ';
	}

	static unsigned countDigits(unsigned u) {
		if(!u)
			return 1u;
		unsigned count = 0u;
		for(; u; u /= 10u)
			++count;
		return count;
	}

	void ConsoleBuildUI::willPerformAction(const ActionDescriptor& action, bool isMinor) {
		if(flags & ConsoleBuildUI::PRINT_DEFINITIVE)
			printAction(getDefinitiveMajorActionCount(), getPredictiveMajorActionCount(), action, isMinor);
		AbstractBuildUI::willPerformAction(action, isMinor);
	}

	void ConsoleBuildUI::wouldPerformAction(const ActionDescriptor& action, bool isMinor) {
		if(flags & ConsoleBuildUI::PRINT_PREDICTIVE)
			printAction(getPredictiveMajorActionCount(), 0u, action, isMinor);
		AbstractBuildUI::wouldPerformAction(action, isMinor);
	}

	void ConsoleBuildUI::printAction(unsigned haveCount, unsigned needCount, const ActionDescriptor& action,
			bool isMinor) {
		if(needCount && haveCount < needCount) {
			if(isMinor)
				indent(countDigits(needCount) * 2u + 11u);
			else {
				if(console) {
					console->setForeground(ConsoleBuildUI::PROGRESS_COLOR);
					console->setIntensity(ConsoleBuildUI::PROGRESS_INTENSITY);
				}
				formatted << '[' << pad<char>(countDigits(needCount), ' ');
				formatted << (haveCount + 1u) << '/' << needCount << " = ";
				unsigned percent = needCount > 1u ? haveCount * 100u / (needCount - 1u) : 100u;
				formatted << pad<char>(3u, ' ') << percent << "%] ";
				if(console)
					console->resetAttributes();
			}
		}
		const string& componentType = action.getComponentType();
		string::size_type ctlen = componentType.length();
		const string& componentName = action.getComponentName();
		string::size_type cnlen = componentName.length();
		if(ctlen && cnlen) {
			if(console) {
				console->setForeground(ConsoleBuildUI::COMPONENT_TAG_COLOR);
				console->setIntensity(ConsoleBuildUI::COMPONENT_TAG_INTENSITY);
			}
			formatted << '[' << componentType;
			if(ctlen < static_cast<string::size_type>(componentTypeWidth))
				indent(static_cast<unsigned>(static_cast<string::size_type>(componentTypeWidth) - ctlen));
			formatted << ' ' << componentName;
			if(cnlen < static_cast<string::size_type>(componentNameWidth))
				indent(static_cast<unsigned>(static_cast<string::size_type>(componentNameWidth) - cnlen));
			formatted << "] ";
			if(console)
				console->resetAttributes();
		}
		else
			indent(componentTypeWidth + componentNameWidth + 4u);
		formatted << action.getActionName();
		const string& source = action.getSource();
		const string& target = action.getTarget();
		if(source.empty()) {
			if(!target.empty())
				formatted << ' ' << target << endln;
		}
		else {
			if(target.empty())
				formatted << ' ' << source << endln;
			else
				formatted << ' ' << source << " -> " << target << endln;
		}
	}

	void ConsoleBuildUI::endPredictiveRun() {
		AbstractBuildUI::endPredictiveRun();
		if(!getPredictiveActionCount() && (flags & ConsoleBuildUI::PRINT_PREDICTIVE))
			formatted << "Nothing to do." << endln;
	}

	void ConsoleBuildUI::endDefinitiveRun() {
		AbstractBuildUI::endDefinitiveRun();
		if(!(flags & ConsoleBuildUI::PRINT_DEFINITIVE))
			return;
		if(!getDefinitiveActionCount()) {
			formatted << "Nothing to do." << endln;
			return;
		}
		unsigned delta = static_cast<unsigned>(getRunDuration());
		unsigned sec = delta % 60u;
		delta /= 60u;
		unsigned min = delta % 60;
		unsigned hrs = delta / 60u;
		if(console) {
			console->setForeground(ConsoleBuildUI::SUCCESS_COLOR);
			console->setIntensity(ConsoleBuildUI::SUCCESS_INTENSITY);
		}
		formatted << pad<char>(2u) << "Build successful in ";
		if(hrs)
			formatted << hrs << ':';
		formatted << min << ':' << sec << endln;
		if(console)
			console->resetAttributes();
	}

}}
