#ifndef REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTWIDGET_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTWIDGET_HPP

#include <set>
#include <redstrain/util/ListenerSet.hpp>
#include <redstrain/util/ListenerList.hpp>

#include "Color.hpp"
#include "Widget.hpp"
#include "FocusListener.hpp"
#include "WidgetListener.hpp"
#include "../Rectangle.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Border;

	class REDSTRAIN_DAMNATION_API AbstractWidget : public virtual Widget {

	  private:
		typedef std::set<Color*> ColorSet;
		typedef ColorSet::const_iterator ColorSetIterator;

	  private:
		Container* parent;
		Layer* layer;
		Rectangle geometry;
		Border* border;
		std::string placementContainer;
		unsigned tabIndex;
		util::ListenerSet<WidgetListener> widgetListeners;
		util::ListenerSet<FocusListener> focusListeners;
		Theme* theme;
		util::ListenerList<InputListener> inputListeners;
		bool opaque;
		ColorSet localColors;

	  protected:
		Color background, inactiveBackground;

	  protected:
		AbstractWidget(const AbstractWidget&);

		inline const Rectangle& getGeometry() const {
			return geometry;
		}

		void addLocalColor(Color&);
		void clearLocalColors();
		Size getSizeWithinBorder() const;
		void setPlacementContainerInColors(const std::string&);

		void fireParentChanged(WidgetListener::ReparentEvent&) const;
		void fireLayerChanged(WidgetListener::RelayerEvent&) const;
		void fireRepositioned(WidgetListener::RepositionEvent&) const;
		void fireResized(WidgetListener::ResizeEvent&) const;
		void fireThemeChanged(WidgetListener::RethemeEvent&) const;

		void fireFocusGained(FocusListener::FocusEvent&) const;
		void fireFocusLost(FocusListener::FocusEvent&) const;
		void fireActiveLayerGained(FocusListener::FocusEvent&) const;
		void fireActiveLayerLost(FocusListener::FocusEvent&) const;

		void propagateInputToAncestors(InputListener::InputEvent&) const;
		bool fireKeyTyped(InputListener::InputEvent&) const;

		virtual void updateChildrenForNewLayer() = 0;
		virtual void clearDescendantColors() = 0;

		virtual Size getMinimumSizeWithinBorder() = 0;
		virtual Size getMaximumSizeWithinBorder() = 0;
		virtual Size getPreferredSizeWithinBorder() = 0;
		virtual unsigned getHeightForWidthWithinBorder(unsigned) = 0;
		virtual unsigned getWidthForHeightWithinBorder(unsigned) = 0;
		virtual void drawIntoBorder(const Rectangle&, DrawContext&) = 0;

		virtual bool isBorderActive() const;
		virtual Widget* getFocusTarget() const;

		virtual void processInputLocally(InputListener::InputEvent&) = 0;
		virtual void clearInputState() = 0;

	  public:
		AbstractWidget(const char*);
		virtual ~AbstractWidget();

		inline Border* getBorder() const {
			return border;
		}

		void setBorder(Border*);

		inline bool isOpaque() const {
			return opaque;
		}

		inline void setOpaque(bool opaque) {
			this->opaque = opaque;
		}

		unsigned getBackgroundColor();
		void setBackgroundColor(unsigned);
		unsigned getInactiveBackgroundColor();
		void setInactiveBackgroundColor(unsigned);

		bool isFocused() const;
		void makeRectVisible(const Rectangle&, Gravity);
		void makeVisible(Gravity);

		virtual Container* getParent();
		virtual void setParent(Container*);
		virtual Screen* getScreen();
		virtual Layer* getLayer();
		virtual void setLayer(Layer*);

		virtual Point getPosition();
		virtual void setPosition(const Point&);
		virtual Size getSize();
		virtual void setSize(const Size&);
		virtual Size getMinimumSize();
		virtual Size getMaximumSize();
		virtual Size getPreferredSize();
		virtual unsigned getHeightForWidth(unsigned);
		virtual unsigned getWidthForHeight(unsigned);
		virtual void repack();

		virtual void draw(const Point&, DrawContext&);
		virtual std::string getPlacementContainer();
		virtual void setPlacementContainer(const std::string&);
		virtual unsigned getTabIndex();
		virtual void setTabIndex(unsigned);

		virtual void getWidgetListeners(util::Appender<WidgetListener*>&);
		virtual bool addWidgetListener(WidgetListener&);
		virtual bool removeWidgetListener(WidgetListener&);
		virtual void clearWidgetListeners();

		virtual void getFocusListeners(util::Appender<FocusListener*>&);
		virtual bool addFocusListener(FocusListener&);
		virtual bool removeFocusListener(FocusListener&);
		virtual void clearFocusListeners();

		virtual void notifyFocusChanged(bool);
		virtual void notifyActiveLayerChanged(bool);

		virtual Theme* getTheme();
		virtual void setTheme(Theme*);
		virtual void clearCachedColors();

		virtual void processInput(InputListener::InputEvent&);

		virtual void getInputListeners(util::Appender<InputListener*>&);
		virtual bool addInputListener(InputListener&);
		virtual bool removeInputListener(InputListener&);
		virtual void clearInputListeners();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTWIDGET_HPP */
