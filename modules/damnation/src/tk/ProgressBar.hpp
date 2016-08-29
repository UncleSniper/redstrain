#ifndef REDSTRAIN_MOD_DAMNATION_TK_PROGRESSBAR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_PROGRESSBAR_HPP

#include <redstrain/util/Tristate.hpp>

#include "Axis.hpp"
#include "LeafWidget.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API ProgressBar : public LeafWidget {

	  public:
		static const unsigned DEFAULT_DRAW_SIZE = 10u;

	  private:
		int minimumValue, maximumValue, currentValue;
		text::String16 caption;
		unsigned drawSize;
		Axis direction;
		Color bar, captionEmptyForeground, captionFilledForeground;
		Color splitCaption, usePartialBlocks, relyOnColor, renderCaption;

	  private:
		void addLocalColors();

	  protected:
		virtual Size getMinimumSizeWithinBorder();
		virtual Size getMaximumSizeWithinBorder();
		virtual Size getPreferredSizeWithinBorder();
		virtual unsigned getHeightForWidthWithinBorder(unsigned);
		virtual unsigned getWidthForHeightWithinBorder(unsigned);
		virtual void drawIntoBorder(const Rectangle&, DrawContext&);

	  public:
		ProgressBar(Axis, const char* = NULL);
		ProgressBar(const ProgressBar&);
		virtual ~ProgressBar();

		inline int getMinimumValue() const {
			return minimumValue;
		}

		inline void setMinimumValue(int minimumValue) {
			this->minimumValue = minimumValue;
		}

		inline int getMaximumValue() const {
			return maximumValue;
		}

		inline void setMaximumValue(int maximumValue) {
			this->maximumValue = maximumValue;
		}

		inline int getCurrentValue() const {
			return currentValue;
		}

		inline void setCurrentValue(int currentValue) {
			this->currentValue = currentValue;
		}

		inline const text::String16& getCaption() const {
			return caption;
		}

		void setCaption(const text::String16&);
		std::string getCaptionUTF8() const;
		void setCaption(const std::string&);

		inline unsigned getDrawSize() const {
			return drawSize;
		}

		void setDrawSize(unsigned);

		inline Axis getDirection() const {
			return direction;
		}

		inline void setDirection(Axis direction) {
			this->direction = direction;
		}

		inline Color& getBar() {
			return bar;
		}

		inline const Color& getBar() const {
			return bar;
		}

		void setBar(const Color&);

		inline Color& getCaptionEmptyForeground() {
			return captionEmptyForeground;
		}

		inline const Color& getCaptionEmptyForeground() const {
			return captionEmptyForeground;
		}

		void setCaptionEmptyForeground(const Color&);

		inline Color& getCaptionFilledForeground() {
			return captionFilledForeground;
		}

		inline const Color& getCaptionFilledForeground() const {
			return captionFilledForeground;
		}

		void setCaptionFilledForeground(const Color&);

		inline Color& getSplitCaption() {
			return splitCaption;
		}

		inline const Color& getSplitCaption() const {
			return splitCaption;
		}

		void setSplitCaption(const Color&);

		inline Color& getUsePartialBlocks() {
			return usePartialBlocks;
		}

		inline const Color& getUsePartialBlocks() const {
			return usePartialBlocks;
		}

		void setUsePartialBlocks(const Color&);

		inline Color& getRelyOnColor() {
			return relyOnColor;
		}

		inline const Color& getRelyOnColor() const {
			return relyOnColor;
		}

		void setRelyOnColor(const Color&);

		inline Color& getRenderCaption() {
			return renderCaption;
		}

		inline const Color& getRenderCaption() const {
			return renderCaption;
		}

		void setRenderCaption(const Color&);

		unsigned getBarColor() const;
		void setBarColor(unsigned);
		unsigned getCaptionEmptyForegroundColor() const;
		void setCaptionEmptyForegroundColor(unsigned);
		unsigned getCaptionFilledForegroundColor() const;
		void setCaptionFilledForegroundColor(unsigned);
		util::Tristate isSplitCaptionOption() const;
		void setSplitCaptionOption(util::Tristate);
		util::Tristate isUsePartialBlocksOption() const;
		void setUsePartialBlocksOption(util::Tristate);
		util::Tristate isRelyOnColorOption() const;
		void setRelyOnColorOption(util::Tristate);
		util::Tristate isRenderCaptionOption() const;
		void setRenderCaptionOption(util::Tristate);

		virtual bool takesFocus();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_PROGRESSBAR_HPP */
