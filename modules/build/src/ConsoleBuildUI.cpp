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
			componentTypeWidth(0u), componentNameWidth(0u), flags(DEFAULT_FLAGS) {}

	ConsoleBuildUI::ConsoleBuildUI(const File::Handle& handle)
			: output(handle), formatted(output), componentTypeWidth(0u), componentNameWidth(0u),
			flags(DEFAULT_FLAGS) {}

	ConsoleBuildUI::ConsoleBuildUI(Console::StandardHandle handle)
			: output(Console::getStandardHandle(handle)), formatted(output), componentTypeWidth(0u),
			componentNameWidth(0u), flags(DEFAULT_FLAGS) {}

	ConsoleBuildUI::ConsoleBuildUI(const ConsoleBuildUI& ui) : AbstractBuildUI(ui),
			output(ui.output.getFile().getHandle()), formatted(output), componentTypeWidth(ui.componentTypeWidth),
			componentNameWidth(ui.componentNameWidth), flags(ui.flags) {}

	void ConsoleBuildUI::willPerformAction(const ActionDescriptor& action) {
		if(flags & ConsoleBuildUI::PRINT_DEFINITIVE)
			printAction(getDefinitiveActionCount(), getPredictiveActionCount(), action);
	}

	void ConsoleBuildUI::wouldPerformAction(const ActionDescriptor& action) {
		if(flags & ConsoleBuildUI::PRINT_PREDICTIVE)
			printAction(getPredictiveActionCount(), 0u, action);
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

	void ConsoleBuildUI::printAction(unsigned haveCount, unsigned needCount, const ActionDescriptor& action) {
		if(needCount && haveCount < needCount) {
			formatted << '[' << pad<char>(countDigits(needCount), ' ');
			formatted << (haveCount + 1u) << '/' << needCount << " = ";
			unsigned percent = needCount > 1u ? haveCount * 100u / (needCount - 1u) : 100u;
			formatted << pad<char>(3u, ' ') << percent << "%] ";
		}
		const string& componentType = action.getComponentType();
		string::size_type ctlen = componentType.length();
		const string& componentName = action.getComponentName();
		string::size_type cnlen = componentName.length();
		formatted << '[' << componentType;
		if(ctlen < static_cast<string::size_type>(componentTypeWidth))
			indent(static_cast<unsigned>(static_cast<string::size_type>(componentTypeWidth) - ctlen));
		formatted << ' ' << componentName;
		if(cnlen < static_cast<string::size_type>(componentNameWidth))
			indent(static_cast<unsigned>(static_cast<string::size_type>(componentNameWidth) - cnlen));
		formatted << "] " << action.getActionName();
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

	void ConsoleBuildUI::endPredictiveRun(bool didSomething) {
		AbstractBuildUI::endPredictiveRun(didSomething);
		if(!didSomething && (flags & ConsoleBuildUI::PRINT_PREDICTIVE))
			formatted << "Nothing to do." << endln;
	}

	void ConsoleBuildUI::endDefinitiveRun(bool didSomething) {
		AbstractBuildUI::endDefinitiveRun(didSomething);
		if(!(flags & ConsoleBuildUI::PRINT_DEFINITIVE))
			return;
		if(!didSomething) {
			formatted << "Nothing to do." << endln;
			return;
		}
		unsigned delta = static_cast<unsigned>(getRunDuration());
		unsigned sec = delta % 60u;
		delta /= 60u;
		unsigned min = delta % 60;
		unsigned hrs = delta / 60u;
		formatted << pad<char>(2u) << "Build successful in ";
		if(hrs)
			formatted << hrs << ':';
		formatted << min << ':' << sec << endln;
	}

}}
