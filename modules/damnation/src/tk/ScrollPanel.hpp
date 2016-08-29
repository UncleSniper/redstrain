#ifndef REDSTRAIN_MOD_DAMNATION_TK_SCROLLPANEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_SCROLLPANEL_HPP

#include "AbstractContainer.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Headed;

	class REDSTRAIN_DAMNATION_API ScrollPanel : public AbstractContainer {

	  public:
		enum REDSTRAIN_DAMNATION_API ScrollBarPolicy {
			SP_ALWAYS,
			SP_WHEN_NEEDED,
			SP_NEVER
		};

	  private:
		Widget* view;
		Headed* headed;
		ScrollBarPolicy verticalScrollBarPolicy, horizontalScrollBarPolicy;
		Size preferredSize;
		Color scrollBarLineForeground, scrollBarLineBackground;
		Color scrollBarBlockForeground, scrollBarBlockBackground;

	  private:
		void addLocalColors();
		void mangleNewView();

	  protected:
		ScrollPanel(const ScrollPanel&);

		virtual Size getMinimumSizeWithinBorder();
		virtual Size getMaximumSizeWithinBorder();
		virtual Size getPreferredSizeWithinBorder();
		virtual unsigned getHeightForWidthWithinBorder(unsigned);
		virtual unsigned getWidthForHeightWithinBorder(unsigned);
		virtual void drawIntoBorder(const Rectangle&, DrawContext&);

		virtual Widget* getFocusTarget() const;
		virtual void makeChildRectVisibleLocally(util::MemorySize, Widget&, const Rectangle&, Gravity);

	  public:
		ScrollPanel(Widget&, const char* = NULL);
		ScrollPanel(Widget&, const Size&, const char* = NULL);
		ScrollPanel(Widget&, ScrollBarPolicy, ScrollBarPolicy, const char* = NULL);
		ScrollPanel(Widget&, const Size&, ScrollBarPolicy, ScrollBarPolicy, const char* = NULL);
		virtual ~ScrollPanel();

		inline Widget& getView() const {
			return *view;
		}

		void setView(Widget&);
		void setPreferredSize(const Size&);

		inline ScrollBarPolicy getVerticalScrollBarPolicy() const {
			return verticalScrollBarPolicy;
		}

		inline void setVerticalScrollBarPolicy(ScrollBarPolicy policy) {
			verticalScrollBarPolicy = policy;
		}

		inline ScrollBarPolicy getHorizontalScrollBarPolicy() const {
			return horizontalScrollBarPolicy;
		}

		inline void setHorizontalScrollBarPolicy(ScrollBarPolicy policy) {
			horizontalScrollBarPolicy = policy;
		}

		inline Color& getScrollBarLineForeground() {
			return scrollBarLineForeground;
		}

		inline const Color& getScrollBarLineForeground() const {
			return scrollBarLineForeground;
		}

		void setScrollBarLineForeground(const Color&);

		inline Color& getScrollBarLineBackground() {
			return scrollBarLineBackground;
		}

		inline const Color& getScrollBarLineBackground() const {
			return scrollBarLineBackground;
		}

		void setScrollBarLineBackground(const Color&);

		inline Color& getScrollBarBlockForeground() {
			return scrollBarBlockForeground;
		}

		inline const Color& getScrollBarBlockForeground() const {
			return scrollBarBlockForeground;
		}

		void setScrollBarBlockForeground(const Color&);

		inline Color& getScrollBarBlockBackground() {
			return scrollBarBlockBackground;
		}

		inline const Color& getScrollBarBlockBackground() const {
			return scrollBarBlockBackground;
		}

		void setScrollBarBlockBackground(const Color&);

		unsigned getScrollBarLineForegroundColor() const;
		void setScrollBarLineForegroundColor(unsigned);
		unsigned getScrollBarLineBackgroundColor() const;
		void setScrollBarLineBackgroundColor(unsigned);
		unsigned getScrollBarBlockForegroundColor() const;
		void setScrollBarBlockForegroundColor(unsigned);
		unsigned getScrollBarBlockBackgroundColor() const;
		void setScrollBarBlockBackgroundColor(unsigned);

		virtual util::MemorySize getChildCount();
		virtual Widget& getChild(util::MemorySize);
		virtual void getChildren(util::Appender<Widget*>&);
		virtual util::MemorySize getIndexOfChild(Widget&);
		virtual bool removeChild(Widget&);
		virtual void pack();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_SCROLLPANEL_HPP */
