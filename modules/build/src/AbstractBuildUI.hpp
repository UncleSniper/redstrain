#ifndef REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP
#define REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP

#include "BuildUI.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API AbstractBuildUI : public BuildUI {

	  private:
		unsigned predictiveActionCount, definitiveActionCount;

	  public:
		AbstractBuildUI();
		AbstractBuildUI(const AbstractBuildUI&);

		inline unsigned getPredictiveActionCount() const {
			return predictiveActionCount;
		}

		inline unsigned getDefinitiveActionCount() const {
			return definitiveActionCount;
		}

		virtual void willPerformAction(const Action&);
		virtual void wouldPerformAction(const Action&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ABSTRACTBUILDUI_HPP */
