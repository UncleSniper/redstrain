#include <redstrain/util/Unref.hpp>
#include <redstrain/util/FireEventAppender.hpp>
#include <redstrain/util/ShortCircuitFireEventAppender.hpp>

#include "Layer.hpp"
#include "Theme.hpp"
#include "Border.hpp"
#include "Screen.hpp"
#include "Container.hpp"
#include "StagePanel.hpp"
#include "DrawContext.hpp"
#include "AbstractWidget.hpp"
#include "CannotRelayerSubtreeError.hpp"
#include "CannotSetParentOfWidgetError.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Appender;
using redengine::util::FireEventAppender;
using redengine::util::ShortCircuitFireEventAppender;

namespace redengine {
namespace damnation {
namespace tk {

	AbstractWidget::AbstractWidget(const char* className) : parent(NULL), layer(NULL), geometry(0, 0, 0u, 0u),
			border(NULL), tabIndex(Layer::INVALID_TAB_INDEX), theme(NULL), opaque(true),
			background(
				"org.unclesniper.damnation.abstract.window.active.bg",
				string(className ? className : "redengine.damnation.tk.AbstractWidget") + ".background.active.bg",
				"windowBackground",
				Color::SIMPLE_BLACK
			),
			inactiveBackground(
				"org.unclesniper.damnation.abstract.window.inactive.bg",
				string(className ? className : "redengine.damnation.tk.AbstractWidget") + ".background.inactive.bg",
				"windowInactiveBackground",
				Color::SIMPLE_BLACK
			) {
		addLocalColor(background);
		addLocalColor(inactiveBackground);
	}

	AbstractWidget::AbstractWidget(const AbstractWidget& widget) : Widget(widget), parent(NULL), layer(NULL),
			geometry(widget.geometry), border(widget.border), placementContainer(widget.placementContainer),
			tabIndex(Layer::INVALID_TAB_INDEX), widgetListeners(widget.widgetListeners),
			focusListeners(widget.focusListeners), theme(widget.theme), inputListeners(widget.inputListeners),
			opaque(widget.opaque), background(widget.background), inactiveBackground(widget.inactiveBackground) {
		addLocalColor(background);
		addLocalColor(inactiveBackground);
		if(border)
			border->ref();
		if(theme)
			theme->ref();
	}

	AbstractWidget::~AbstractWidget() {
		if(border)
			border->unref();
		if(theme)
			theme->unref();
		if(layer)
			layer->removeTabIndexOfDestroyedWidget(*this);
	}

	void AbstractWidget::setBorder(Border* border) {
		if(border)
			border->ref();
		if(this->border)
			this->border->unref();
		this->border = border;
	}

	void AbstractWidget::setBackground(const Color& color) {
		background = color;
	}

	void AbstractWidget::setInactiveBackground(const Color& color) {
		inactiveBackground = color;
	}

	unsigned AbstractWidget::getBackgroundColor() {
		return background.getLocal();
	}

	void AbstractWidget::setBackgroundColor(unsigned color) {
		background.setLocal(color);
	}

	unsigned AbstractWidget::getInactiveBackgroundColor() {
		return inactiveBackground.getLocal();
	}

	void AbstractWidget::setInactiveBackgroundColor(unsigned color) {
		inactiveBackground.setLocal(color);
	}

	bool AbstractWidget::isFocused() const {
		if(!layer)
			return false;
		const Screen& screen = layer->getScreen();
		if(screen.getActiveLayer() != layer)
			return false;
		Widget* focus = layer->getCurrentFocus();
		if(focus == this)
			return true;
		Widget* target = getFocusTarget();
		return target && target == focus;
	}

	void AbstractWidget::makeRectVisible(const Rectangle& rectangle, Gravity gravity) {
		if(parent)
			parent->makeChildRectVisible(*this, rectangle, gravity);
	}

	void AbstractWidget::makeVisible(Gravity gravity) {
		if(parent)
			parent->makeChildVisible(*this, gravity);
	}

	void AbstractWidget::addLocalColor(Color& color) {
		localColors.insert(&color);
	}

	void AbstractWidget::clearLocalColors() {
		ColorSetIterator begin(localColors.begin()), end(localColors.end());
		for(; begin != end; ++begin)
			(*begin)->clearCache();
	}

	Size AbstractWidget::getSizeWithinBorder() const {
		Size size(geometry.width, geometry.height);
		if(border) {
			Insets insets(border->getBorderSize());
			unsigned width = insets.left + insets.right;
			if(width >= size.width)
				size.width = 0u;
			else
				size.width -= width;
			unsigned height = insets.top + insets.bottom;
			if(height >= size.height)
				size.height = 0u;
			else
				size.height -= height;
		}
		return size;
	}

	void AbstractWidget::setPlacementContainerInColors(const string& placementContainer) {
		ColorSetIterator begin(localColors.begin()), end(localColors.end());
		for(; begin != end; ++begin)
			(*begin)->setContainer(placementContainer);
	}

#define makeFireEvent(name, listenerT, listeners, callback, eventT) \
	void AbstractWidget::name(listenerT::eventT& event) const { \
		FireEventAppender<listenerT, listenerT::eventT> sink(&listenerT::callback, event); \
		listeners.getListeners(sink); \
	}

	makeFireEvent(fireParentChanged, WidgetListener, widgetListeners, parentChanged, ReparentEvent)
	makeFireEvent(fireLayerChanged, WidgetListener, widgetListeners, layerChanged, RelayerEvent)
	makeFireEvent(fireRepositioned, WidgetListener, widgetListeners, repositioned, RepositionEvent)
	makeFireEvent(fireResized, WidgetListener, widgetListeners, resized, ResizeEvent)
	makeFireEvent(fireThemeChanged, WidgetListener, widgetListeners, themeChanged, RethemeEvent)
	makeFireEvent(fireFocusGained, FocusListener, focusListeners, focusGained, FocusEvent)
	makeFireEvent(fireFocusLost, FocusListener, focusListeners, focusLost, FocusEvent)
	makeFireEvent(fireActiveLayerGained, FocusListener, focusListeners, activeLayerGained, FocusEvent)
	makeFireEvent(fireActiveLayerLost, FocusListener, focusListeners, activeLayerLost, FocusEvent)

	void AbstractWidget::propagateInputToAncestors(InputListener::InputEvent& event) const {
		if(parent)
			parent->processInput(event);
	}

	bool AbstractWidget::fireKeyTyped(InputListener::InputEvent& event) const {
		ShortCircuitFireEventAppender<InputListener, InputListener::InputEvent>
				sink(&InputListener::keyTyped, event, true, true);
		inputListeners.getListeners(sink);
		return sink.getResult();
	}

	bool AbstractWidget::isBorderActive() const {
		return isFocused();
	}

	Widget* AbstractWidget::getFocusTarget() const {
		return NULL;
	}

	Container* AbstractWidget::getParent() {
		return parent;
	}

	void AbstractWidget::setParent(Container* newParent) {
		if(parent == newParent)
			return;
		if(newParent && newParent->getIndexOfChild(*this) == Container::INVALID_CHILD_INDEX)
			throw CannotSetParentOfWidgetError();
		WidgetListener::ReparentEvent event(*this, parent, newParent);
		Container* oldParent = parent;
		parent = newParent;
		if(oldParent)
			oldParent->removeChild(*this);
		setLayer(parent ? parent->getLayer() : NULL);
		fireParentChanged(event);
	}

	Screen* AbstractWidget::getScreen() {
		return layer ? &layer->getScreen() : NULL;
	}

	Layer* AbstractWidget::getLayer() {
		return layer;
	}

	void AbstractWidget::setLayer(Layer* newLayer) {
		if(layer == newLayer)
			return;
		StagePanel* oldStage = NULL;
		if(parent && parent->getLayer() != newLayer) {
			Layer* player = parent->getLayer();
			if(!player || parent != &player->getStage())
				throw CannotRelayerSubtreeError();
			oldStage = &player->getStage();
		}
		WidgetListener::RelayerEvent event(*this, layer, newLayer);
		if(layer)
			layer->removeTabIndex(*this);
		layer = newLayer;
		tabIndex = Layer::INVALID_TAB_INDEX;
		if(newLayer && takesFocus())
			newLayer->addTabIndex(*this);
		updateChildrenForNewLayer();
		if(newLayer) {
			const StageConstraints* oldsc = oldStage ? oldStage->getConstraint(*this) : NULL;
			newLayer->getStage().add(*this, oldsc ? *oldsc : StageConstraints());
		}
		fireLayerChanged(event);
	}

	Point AbstractWidget::getPosition() {
		return Point(geometry.row, geometry.column);
	}

	void AbstractWidget::setPosition(const Point& position) {
		WidgetListener::RepositionEvent event(*this, Point(geometry.row, geometry.column), position);
		geometry.row = position.row;
		geometry.column = position.column;
		fireRepositioned(event);
	}

	Size AbstractWidget::getSize() {
		return Size(geometry.width, geometry.height);
	}

	void AbstractWidget::setSize(const Size& size) {
		WidgetListener::ResizeEvent event(*this, Size(geometry.width, geometry.height), size);
		geometry.width = size.width;
		geometry.height = size.height;
		fireResized(event);
	}

	Size AbstractWidget::getMinimumSize() {
		Size size(getMinimumSizeWithinBorder());
		if(border) {
			Insets insets(border->getBorderSize());
			size.width += insets.left + insets.right;
			size.height += insets.top + insets.bottom;
		}
		return size;
	}

	Size AbstractWidget::getMaximumSize() {
		Size size(getMaximumSizeWithinBorder());
		if(border) {
			Insets insets(border->getBorderSize());
			if(size.width != Widget::NO_SIZE_LIMIT)
				size.width += insets.left + insets.right;
			if(size.height != Widget::NO_SIZE_LIMIT)
				size.height += insets.top + insets.bottom;
		}
		return size;
	}

	Size AbstractWidget::getPreferredSize() {
		Size size(getPreferredSizeWithinBorder());
		if(border) {
			Insets insets(border->getBorderSize());
			size.width += insets.left + insets.right;
			size.height += insets.top + insets.bottom;
		}
		return size;
	}

	unsigned AbstractWidget::getHeightForWidth(unsigned width) {
		if(!border)
			return getHeightForWidthWithinBorder(width);
		Insets insets(border->getBorderSize());
		unsigned iwidth = insets.left + insets.right;
		if(iwidth >= width)
			width = 0u;
		else
			width -= iwidth;
		return getHeightForWidthWithinBorder(width) + insets.top + insets.bottom;
	}

	unsigned AbstractWidget::getWidthForHeight(unsigned height) {
		if(!border)
			return getWidthForHeightWithinBorder(height);
		Insets insets(border->getBorderSize());
		unsigned iheight = insets.top + insets.bottom;
		if(iheight >= height)
			height = 0u;
		else
			height -= iheight;
		return getWidthForHeightWithinBorder(height) + insets.left + insets.right;
	}

	void AbstractWidget::repack() {
		if(parent)
			parent->pack();
		else
			pack();
	}

	void AbstractWidget::draw(const Point& position, DrawContext& context) {
		context.pushTheme(theme);
		DrawContext::PopTheme popTheme(&context);
		const Rectangle targetRect(position, geometry.width, geometry.height);
		if(opaque) {
			TerminalCanvas& canvas = context.getCanvas();
			canvas.setBackgroundColor((isFocused() ? background : inactiveBackground).get(context));
			canvas.fill(targetRect, ' ');
		}
		if(border) {
			border->drawBorder(targetRect, isBorderActive(), context);
			Insets insets(border->getBorderSize());
			unsigned bwidth = insets.left + insets.right, bheight = insets.top + insets.bottom;
			drawIntoBorder(Rectangle(
				position.row + static_cast<int>(insets.top),
				position.column + static_cast<int>(insets.left),
				geometry.width > bwidth ? geometry.width - bwidth : 0u,
				geometry.height > bheight ? geometry.height - bheight : 0u
			), context);
		}
		else
			drawIntoBorder(targetRect, context);
		popTheme.set();
	}

	string AbstractWidget::getPlacementContainer() {
		return placementContainer;
	}

	void AbstractWidget::setPlacementContainer(const string& placementContainer) {
		this->placementContainer = placementContainer;
		setPlacementContainerInColors(placementContainer);
	}

	unsigned AbstractWidget::getTabIndex() {
		return tabIndex;
	}

	void AbstractWidget::setTabIndex(unsigned index) {
		if(!layer || tabIndex == index)
			return;
		tabIndex = index;
		if(index != Layer::INVALID_TAB_INDEX)
			layer->setTabIndex(*this, index);
		else
			layer->removeTabIndex(*this);
	}

#define makeListenerFuncs(listenerT, listeners) \
	void AbstractWidget::get ## listenerT ## s(Appender<listenerT*>& sink) { \
		listeners.getListeners(sink); \
	} \
	bool AbstractWidget::add ## listenerT(listenerT& listener) { \
		return listeners.addListener(listener); \
	} \
	bool AbstractWidget::remove ## listenerT(listenerT& listener) { \
		return listeners.removeListener(listener); \
	} \
	void AbstractWidget::clear ## listenerT ## s() { \
		listeners.clearListeners(); \
	}

	makeListenerFuncs(WidgetListener, widgetListeners)
	makeListenerFuncs(FocusListener, focusListeners)
	makeListenerFuncs(InputListener, inputListeners)

	void AbstractWidget::notifyFocusChanged(bool isFocused) {
		FocusListener::FocusEvent event(*this);
		if(isFocused)
			fireFocusGained(event);
		else {
			clearInputState();
			fireFocusLost(event);
		}
	}

	void AbstractWidget::notifyActiveLayerChanged(bool isActive) {
		FocusListener::FocusEvent event(*this);
		if(isActive)
			fireActiveLayerGained(event);
		else {
			clearInputState();
			fireActiveLayerLost(event);
		}
	}

	Theme* AbstractWidget::getTheme() {
		return theme;
	}

	void AbstractWidget::setTheme(Theme* theme) {
		if(this->theme == theme)
			return;
		WidgetListener::RethemeEvent event(*this, this->theme, theme);
		Unref<Theme> oldTheme(this->theme);
		this->theme = theme;
		theme->ref();
		clearCachedColors();
		fireThemeChanged(event);
	}

	void AbstractWidget::clearCachedColors() {
		clearLocalColors();
		clearDescendantColors();
	}

	void AbstractWidget::processInput(InputListener::InputEvent& event) {
		if(!fireKeyTyped(event))
			processInputLocally(event);
	}

}}}
