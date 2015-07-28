#include "Action.hpp"
#include "AbstractBuildUI.hpp"

namespace redengine {
namespace build {

	AbstractBuildUI::AbstractBuildUI()
			: predictiveActionCount(0u), definitiveActionCount(0u), startOfRun(static_cast<time_t>(0u)) {}

	AbstractBuildUI::AbstractBuildUI(const AbstractBuildUI& ui) : BuildUI(ui),
			predictiveActionCount(ui.predictiveActionCount), definitiveActionCount(ui.definitiveActionCount),
			startOfRun(ui.startOfRun) {}

	time_t AbstractBuildUI::getRunDuration() const {
		return time(NULL) - startOfRun;
	}

	void AbstractBuildUI::willPerformAction(const Action& action) {
		action.notifyUIWillPerform(*this);
		++definitiveActionCount;
	}

	void AbstractBuildUI::wouldPerformAction(const Action& action) {
		action.notifyUIWouldPerform(*this);
		++predictiveActionCount;
	}

	void AbstractBuildUI::startPredictiveRun() {
		startOfRun = time(NULL);
	}

	void AbstractBuildUI::endPredictiveRun(bool) {}

	void AbstractBuildUI::startDefinitiveRun() {
		startOfRun = time(NULL);
	}

	void AbstractBuildUI::endDefinitiveRun(bool) {}

}}
