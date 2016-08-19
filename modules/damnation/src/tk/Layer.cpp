#include "Layer.hpp"
#include "StagePanel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	Layer::Layer(Screen& screen, unsigned depth) : screen(screen), depth(depth), stage(new StagePanel),
			currentTabIndex(0u) {
		stage->setOpaque(!depth);
		stage->setLayer(*this);
	}

	Layer::Layer(const Layer& layer) : screen(layer.screen), depth(layer.depth), stage(NULL),
			currentTabIndex(Layer::INVALID_TAB_INDEX) {}

	Layer::~Layer() {
		delete stage;
	}

	unsigned Layer::addTabIndex(Widget& widget) {
		//TODO
	}

	unsigned Layer::removeTabIndex(Widget& widget) {
		//TODO
	}

	void Layer::setTabIndex(Widget& widget, unsigned index) {
		//TODO
	}

	Widget* Layer::getCurrentFocus() const {
		//TODO
	}

	void Layer::setCurrentTabIndex(unsigned index) {
		//TODO
	}

	void Layer::shiftFocus() {
		//TODO
	}

	void Layer::unshiftFocus() {
		//TODO
	}

	void Layer::notifyActiveChanged(bool isActive) {
		//TODO
	}

}}}
