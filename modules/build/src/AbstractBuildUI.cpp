#include "Action.hpp"
#include "AbstractBuildUI.hpp"

namespace redengine {
namespace build {

	AbstractBuildUI::AbstractBuildUI() {}

	AbstractBuildUI::AbstractBuildUI(const AbstractBuildUI& ui) : BuildUI(ui),
			predictiveActionCount(ui.predictiveActionCount), definitiveActionCount(ui.definitiveActionCount) {}

	void AbstractBuildUI::willPerformAction(const Action& action) {
		action.notifyUIWillPerform(*this);
		++definitiveActionCount;
	}

	void AbstractBuildUI::wouldPerformAction(const Action& action) {
		action.notifyUIWouldPerform(*this);
		++predictiveActionCount;
	}

}}