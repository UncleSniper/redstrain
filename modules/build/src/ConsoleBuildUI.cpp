#include <redstrain/io/streamoperators.hpp>

#include "ConsoleBuildUI.hpp"

using std::string;
using redengine::platform::File;
using redengine::platform::Console;
using redengine::io::FileOutputStream;
using redengine::io::endln;
using redengine::io::operator<<;

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
		printAction(getDefinitiveActionCount(), getPredictiveActionCount(), action);
	}

	void ConsoleBuildUI::wouldPerformAction(const ActionDescriptor& action) {
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
		if(needCount) {
			formatted << '[';
			indent(countDigits(needCount) - countDigits(haveCount + 1u));
			formatted << (haveCount + 1u) << '/' << needCount << " = ";
			unsigned percent = needCount > 1u ? haveCount * 100u / (needCount - 1u) : 100u;
			indent(3u - countDigits(percent));
			formatted << percent << "%] ";
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

}}
