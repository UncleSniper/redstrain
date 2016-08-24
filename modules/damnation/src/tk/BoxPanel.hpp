#ifndef REDSTRAIN_MOD_DAMNATION_TK_BOXPANEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_BOXPANEL_HPP

#include "Axis.hpp"
#include "Alignment.hpp"
#include "SimplePanel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API BoxPanel : public SimplePanel {

	  private:
		Axis direction;
		Alignment alignment, excessAlignment;

	  protected:
		virtual Size getMinimumSizeWithinBorder();
		virtual Size getMaximumSizeWithinBorder();
		virtual Size getPreferredSizeWithinBorder();
		virtual unsigned getHeightForWidthWithinBorder(unsigned);
		virtual unsigned getWidthForHeightWithinBorder(unsigned);

	  public:
		BoxPanel(Axis, const char* = NULL);
		BoxPanel(const BoxPanel&);

		inline Axis getDirection() const {
			return direction;
		}

		inline void setDirection(Axis direction) {
			this->direction = direction;
		}

		inline Alignment getAlignment() const {
			return alignment;
		}

		inline void setAlignment(Alignment alignment) {
			this->alignment = alignment;
		}

		inline Alignment getExcessAlignment() const {
			return excessAlignment;
		}

		inline void setExcessAlignment(Alignment excessAlignment) {
			this->excessAlignment = excessAlignment;
		}

		virtual void pack();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_BOXPANEL_HPP */
