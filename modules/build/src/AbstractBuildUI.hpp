#ifndef REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP
#define REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP

#include <ctime>

#include "BuildUI.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API AbstractBuildUI : public BuildUI {

	  private:
		unsigned predictiveActionCount, definitiveActionCount;
		time_t startOfRun;

	  protected:
		time_t getRunDuration() const;

	  public:
		AbstractBuildUI();
		AbstractBuildUI(const AbstractBuildUI&);

		inline unsigned getPredictiveActionCount() const {
			return predictiveActionCount;
		}

		inline unsigned getDefinitiveActionCount() const {
			return definitiveActionCount;
		}

		inline time_t getStartOfRunTimestamp() const {
			return startOfRun;
		}

		virtual void willPerformAction(const Action&);
		virtual void wouldPerformAction(const Action&);
		virtual void startPredictiveRun();
		virtual void endPredictiveRun(bool);
		virtual void startDefinitiveRun();
		virtual void endDefinitiveRun(bool);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP */
