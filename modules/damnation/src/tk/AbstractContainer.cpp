#include "Border.hpp"
#include "AbstractContainer.hpp"
#include "util/SetWidgetLayerAppender.hpp"
#include "util/ClearWidgetCachedColorsAppender.hpp"

using redengine::util::MemorySize;
using redengine::damnation::tk::util::SetWidgetLayerAppender;
using redengine::damnation::tk::util::ClearWidgetCachedColorsAppender;

namespace redengine {
namespace damnation {
namespace tk {

	AbstractContainer::AbstractContainer(const char* className)
			: AbstractWidget(className ? className : "redengine.damnation.tk.AbstractContainer") {}

	AbstractContainer::AbstractContainer(const AbstractContainer& container)
			: Widget(container), Container(container), AbstractWidget(container) {}

	void AbstractContainer::updateChildrenForNewLayer() {
		SetWidgetLayerAppender sink(getLayer());
		getChildren(sink);
	}

	void AbstractContainer::clearDescendantColors() {
		ClearWidgetCachedColorsAppender sink;
		getChildren(sink);
	}

	void AbstractContainer::processInputLocally(InputListener::InputEvent& event) {
		propagateInputToAncestors(event);
	}

	void AbstractContainer::clearInputState() {}

	bool AbstractContainer::takesFocus() {
		return false;
	}

	void AbstractContainer::makeChildRectVisible(Widget& child, const Rectangle& rectangle, Gravity gravity) {
		Rectangle prect(rectangle);
		MemorySize index = getIndexOfChild(child);
		if(index != Container::INVALID_CHILD_INDEX) {
			makeChildRectVisibleLocally(index, child, rectangle, gravity);
			const Point position(child.getPosition());
			prect.row += position.row;
			prect.column += position.column;
			Border* border = getBorder();
			if(border) {
				const Insets insets(border->getBorderSize());
				prect.row += static_cast<int>(insets.top);
				prect.column += static_cast<int>(insets.left);
			}
		}
		Container* parent = getParent();
		if(parent)
			parent->makeChildRectVisible(*this, prect, gravity);
	}

	void AbstractContainer::makeChildRectVisibleLocally(MemorySize, Widget&, const Rectangle&, Gravity) {}

	void AbstractContainer::makeChildVisible(Widget& child, Gravity gravity) {
		MemorySize index = getIndexOfChild(child);
		if(index != Container::INVALID_CHILD_INDEX)
			makeChildVisibleLocally(index, child, gravity);
		Container* parent = getParent();
		if(parent)
			parent->makeChildVisible(*this, gravity);
	}

	void AbstractContainer::makeChildVisibleLocally(MemorySize, Widget&, Gravity) {}

}}}
