#ifndef REDSTRAIN_MOD_DAMNATION_TK_WIDGET_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_WIDGET_HPP

#include <string>
#include <redstrain/util/Appender.hpp>

#include "../Size.hpp"
#include "../Point.hpp"
#include "InputListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Layer;
	class Theme;
	class Screen;
	class Container;
	class DrawContext;
	class FocusListener;
	class WidgetListener;

	class REDSTRAIN_DAMNATION_API Widget {

	  public:
		static const unsigned NO_SIZE_LIMIT = ~0u;

	  protected:
		Widget(const Widget&);

	  public:
		Widget();
		virtual ~Widget();

		Point getAbsolutePosition();

		virtual Container* getParent() = 0;
		virtual void setParent(Container*) = 0;
		virtual Screen* getScreen() = 0;
		virtual Layer* getLayer() = 0;
		virtual void setLayer(Layer*) = 0;

		virtual Point getPosition() = 0;
		virtual void setPosition(const Point&) = 0;
		virtual Size getSize() = 0;
		virtual void setSize(const Size&) = 0;
		virtual Size getMinimumSize() = 0;
		virtual Size getMaximumSize() = 0;
		virtual Size getPreferredSize() = 0;
		virtual unsigned getHeightForWidth(unsigned) = 0;
		virtual unsigned getWidthForHeight(unsigned) = 0;
		virtual void pack() = 0;
		virtual void repack() = 0;

		virtual void draw(const Point&, DrawContext&) = 0;
		virtual std::string getPlacementContainer() = 0;
		virtual void setPlacementContainer(const std::string&) = 0;
		virtual bool takesFocus() = 0;
		virtual unsigned getTabIndex() = 0;
		virtual void setTabIndex(unsigned) = 0;

		virtual void getWidgetListeners(util::Appender<WidgetListener*>&) = 0;
		virtual bool addWidgetListener(WidgetListener&) = 0;
		virtual bool removeWidgetListener(WidgetListener&) = 0;
		virtual void clearWidgetListeners() = 0;

		virtual void getFocusListeners(util::Appender<FocusListener*>&) = 0;
		virtual bool addFocusListener(FocusListener&) = 0;
		virtual bool removeFocusListener(FocusListener&) = 0;
		virtual void clearFocusListeners() = 0;

		virtual void notifyFocusChanged(bool) = 0;
		virtual void notifyActiveLayerChanged(bool) = 0;

		virtual Theme* getTheme() = 0;
		virtual void setTheme(Theme*) = 0;
		virtual void clearCachedColors() = 0;

		virtual void processInput(InputListener::InputEvent&) = 0;

		virtual void getInputListeners(util::Appender<InputListener*>&) = 0;
		virtual bool addInputListener(InputListener&) = 0;
		virtual bool removeInputListener(InputListener&) = 0;
		virtual void clearInputListeners() = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_WIDGET_HPP */
