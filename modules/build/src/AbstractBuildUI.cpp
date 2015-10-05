#include "AbstractBuildUI.hpp"

namespace redengine {
namespace build {

	AbstractBuildUI::AbstractBuildUI() : predictiveMajorActionCount(0u), predictiveMinorActionCount(0u),
			definitiveMajorActionCount(0u), definitiveMinorActionCount(0u), startOfRun(static_cast<time_t>(0u)) {}

	AbstractBuildUI::AbstractBuildUI(const AbstractBuildUI& ui) : BuildUI(ui),
			predictiveMajorActionCount(ui.predictiveMajorActionCount),
			predictiveMinorActionCount(ui.predictiveMinorActionCount),
			definitiveMajorActionCount(ui.definitiveMajorActionCount),
			definitiveMinorActionCount(ui.definitiveMinorActionCount),
			startOfRun(ui.startOfRun) {}

	time_t AbstractBuildUI::getRunDuration() const {
		return time(NULL) - startOfRun;
	}

	void AbstractBuildUI::willPerformAction(const ActionDescriptor&, bool isMinor) {
		if(isMinor)
			++definitiveMinorActionCount;
		else
			++definitiveMajorActionCount;
	}

	void AbstractBuildUI::wouldPerformAction(const ActionDescriptor&, bool isMinor) {
		if(isMinor)
			++predictiveMinorActionCount;
		else
			++predictiveMajorActionCount;
	}

	void AbstractBuildUI::startPredictiveRun() {
		startOfRun = time(NULL);
	}

	void AbstractBuildUI::endPredictiveRun() {}

	void AbstractBuildUI::startDefinitiveRun() {
		startOfRun = time(NULL);
	}

	void AbstractBuildUI::endDefinitiveRun() {}

}}
