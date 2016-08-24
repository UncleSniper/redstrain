#include "Container.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	Widget::Widget() {}

	Widget::Widget(const Widget&) {}

	Widget::~Widget() {}

	Point Widget::getAbsolutePosition() {
		const Point position(getPosition());
		Container* parent = getParent();
		return parent ? position + parent->getAbsolutePosition() : position;
	}

}}}
