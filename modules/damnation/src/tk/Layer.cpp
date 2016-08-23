#include "Layer.hpp"
#include "Screen.hpp"
#include "StagePanel.hpp"
#include "TabIndexOutOfBoundsError.hpp"
#include "WidgetForeignToLayerError.hpp"

using std::vector;
using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace tk {

	Layer::Layer(Screen& screen, unsigned depth) : screen(screen), depth(depth), stage(new StagePanel),
			currentTabIndex(0u), currentlyRemoving(false) {
		stage->setOpaque(!depth);
		stage->setLayer(this);
	}

	Layer::Layer(const Layer& layer) : screen(layer.screen), depth(layer.depth), stage(NULL),
			currentTabIndex(Layer::INVALID_TAB_INDEX), currentlyRemoving(false) {}

	Layer::~Layer() {
		delete stage;
	}

	bool Layer::remove() {
		if(depth == Layer::INVALID_LAYER_DEPTH || currentlyRemoving)
			return false;
		currentlyRemoving = true;
		screen.removeLayer(*this);
		currentlyRemoving = false;
		depth = Layer::INVALID_LAYER_DEPTH;
		return true;
	}

	struct PopBackLayerTab {

		vector<Widget*>* tabs;

		PopBackLayerTab(vector<Widget*>* tabs) : tabs(tabs) {}

		PopBackLayerTab(const PopBackLayerTab& pop) : tabs(pop.tabs) {}

		~PopBackLayerTab() {
			if(tabs)
				tabs->pop_back();
		}

	};

	unsigned Layer::addTabIndex(Widget& widget) {
		if(widget.getLayer() != this)
			throw WidgetForeignToLayerError();
		InverseTabs::const_iterator present(inverseTabs.find(&widget));
		if(present != inverseTabs.end())
			return present->second;
		unsigned index = static_cast<unsigned>(tabs.size());
		tabs.push_back(&widget);
		PopBackLayerTab popTab(&tabs);
		inverseTabs[&widget] = index;
		popTab.tabs = NULL;
		widget.setTabIndex(index);
		if(!index)
			widget.notifyFocusChanged(true);
		return index;
	}

	unsigned Layer::removeTabIndex(Widget& widget, bool mayUseWidget) {
		if(mayUseWidget && widget.getLayer() != this)
			return Layer::INVALID_TAB_INDEX;
		InverseTabs::iterator present(inverseTabs.find(&widget));
		if(present == inverseTabs.end())
			return Layer::INVALID_TAB_INDEX;
		const unsigned oldIndex = present->second;
		tabs.erase(tabs.begin() + static_cast<Tabs::size_type>(oldIndex));
		inverseTabs.erase(present);
		if(mayUseWidget)
			widget.setTabIndex(Layer::INVALID_TAB_INDEX);
		unsigned u, size = static_cast<unsigned>(tabs.size());
		for(u = oldIndex; u < size; ++u) {
			Widget& w = *tabs[static_cast<Tabs::size_type>(u)];
			inverseTabs[&w] = u;
			w.setTabIndex(u);
		}
		if(currentTabIndex == oldIndex) {
			if(!size)
				currentTabIndex = 0u;
			else if(!currentTabIndex)
				currentTabIndex = size - 1u;
			else
				--currentTabIndex;
			if(mayUseWidget)
				widget.notifyFocusChanged(false);
			Widget* newFocus = getCurrentFocus();
			if(newFocus)
				newFocus->notifyFocusChanged(true);
		}
		else if(currentTabIndex > oldIndex)
			--currentTabIndex;
		return oldIndex;
	}

	unsigned Layer::removeTabIndex(Widget& widget) {
		return removeTabIndex(widget, true);
	}

	void Layer::removeTabIndexOfDestroyedWidget(Widget& widget) {
		removeTabIndex(widget, false);
	}

	void Layer::setTabIndex(Widget& widget, unsigned index) {
		if(widget.getLayer() != this)
			throw WidgetForeignToLayerError();
		InverseTabs::iterator present(inverseTabs.find(&widget));
		if(present == inverseTabs.end())
			return;
		const unsigned oldIndex = present->second;
		if(oldIndex == index)
			return;
		if(index >= static_cast<unsigned>(tabs.size()))
			throw TabIndexOutOfBoundsError(static_cast<MemorySize>(index));
		Widget& other = *tabs[static_cast<Tabs::size_type>(index)];
		tabs[static_cast<Tabs::size_type>(oldIndex)] = &other;
		tabs[static_cast<Tabs::size_type>(index)] = &widget;
		inverseTabs[&widget] = index;
		inverseTabs[&other] = oldIndex;
		widget.setTabIndex(index);
		other.setTabIndex(oldIndex);
		if(currentTabIndex == oldIndex)
			currentTabIndex = index;
		else if(currentTabIndex == index)
			currentTabIndex = oldIndex;
	}

	Widget* Layer::getCurrentFocus() const {
		if(static_cast<Tabs::size_type>(currentTabIndex) < tabs.size())
			return tabs[static_cast<Tabs::size_type>(currentTabIndex)];
		else
			return NULL;
	}

	void Layer::setCurrentTabIndex(unsigned index) {
		if(tabs.empty())
			currentTabIndex = 0u;
		else {
			Widget& oldFocus = *tabs[static_cast<Tabs::size_type>(currentTabIndex)];
			currentTabIndex = index % static_cast<unsigned>(tabs.size());
			Widget& newFocus = *tabs[static_cast<Tabs::size_type>(currentTabIndex)];
			if(&newFocus != &oldFocus) {
				oldFocus.notifyFocusChanged(false);
				newFocus.notifyFocusChanged(true);
			}
		}
	}

	void Layer::shiftFocus() {
		if(tabs.empty())
			currentTabIndex = 0u;
		else {
			Widget& oldFocus = *tabs[static_cast<Tabs::size_type>(currentTabIndex)];
			currentTabIndex = (currentTabIndex + 1u) % static_cast<unsigned>(tabs.size());
			Widget& newFocus = *tabs[static_cast<Tabs::size_type>(currentTabIndex)];
			if(&newFocus != &oldFocus) {
				oldFocus.notifyFocusChanged(false);
				newFocus.notifyFocusChanged(true);
			}
		}
	}

	void Layer::unshiftFocus() {
		if(tabs.empty())
			currentTabIndex = 0u;
		else {
			Widget& oldFocus = *tabs[static_cast<Tabs::size_type>(currentTabIndex)];
			if(currentTabIndex)
				--currentTabIndex;
			else
				currentTabIndex = static_cast<unsigned>(tabs.size()) - 1u;
			Widget& newFocus = *tabs[static_cast<Tabs::size_type>(currentTabIndex)];
			if(&newFocus != &oldFocus) {
				oldFocus.notifyFocusChanged(false);
				newFocus.notifyFocusChanged(true);
			}
		}
	}

	void Layer::notifyActiveChanged(bool isActive) {
		Widget* focus = getCurrentFocus();
		if(focus)
			focus->notifyActiveLayerChanged(isActive);
	}

	void Layer::notifyDepthChanged() {
		if(depth == Layer::INVALID_LAYER_DEPTH || currentlyRemoving)
			return;
		unsigned newDepth = screen.getTrueDepthOfLayer(*this);
		if(newDepth != Layer::INVALID_LAYER_DEPTH)
			depth = newDepth;
	}

}}}
