#ifndef REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP
#define REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP

#include <ctime>

#include "BuildUI.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API AbstractBuildUI : public BuildUI {

	  private:
		unsigned predictiveMajorActionCount, predictiveMinorActionCount;
		unsigned definitiveMajorActionCount, definitiveMinorActionCount;
		time_t startOfRun;

	  protected:
		time_t getRunDuration() const;

	  public:
		AbstractBuildUI();
		AbstractBuildUI(const AbstractBuildUI&);

		inline unsigned getPredictiveMajorActionCount() const {
			return predictiveMajorActionCount;
		}

		inline unsigned getPredictiveMinorActionCount() const {
			return predictiveMinorActionCount;
		}

		inline unsigned getPredictiveActionCount() const {
			return predictiveMajorActionCount + predictiveMinorActionCount;
		}

		inline unsigned getDefinitiveMajorActionCount() const {
			return definitiveMajorActionCount;
		}

		inline unsigned getDefinitiveMinorActionCount() const {
			return definitiveMinorActionCount;
		}

		inline unsigned getDefinitiveActionCount() const {
			return definitiveMajorActionCount + definitiveMinorActionCount;
		}

		inline time_t getStartOfRunTimestamp() const {
			return startOfRun;
		}

		virtual void willPerformAction(const ActionDescriptor&, bool);
		virtual void wouldPerformAction(const ActionDescriptor&, bool);
		virtual void startPredictiveRun();
		virtual void endPredictiveRun();
		virtual void startDefinitiveRun();
		virtual void endDefinitiveRun();

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP */
