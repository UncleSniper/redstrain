#ifndef REDSTRAIN_MOD_DAMNATION_TK_STAGEPANEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_STAGEPANEL_HPP

#include "ConstraintPanel.hpp"
#include "StageConstraints.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API StagePanel : public ConstraintPanel<StageConstraints> {

	  protected:
		virtual Size getMinimumSizeWithinBorder();
		virtual Size getMaximumSizeWithinBorder();
		virtual Size getPreferredSizeWithinBorder();
		virtual unsigned getHeightForWidthWithinBorder(unsigned);
		virtual unsigned getWidthForHeightWithinBorder(unsigned);

	  public:
		StagePanel(const char* = NULL);
		StagePanel(const StagePanel&);

		virtual void pack();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_STAGEPANEL_HPP */
