#include "Insets.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	Insets::Insets(unsigned top, unsigned left, unsigned right, unsigned bottom)
			: top(top), left(left), right(right), bottom(bottom) {}

	Insets::Insets(const Insets& insets)
			: top(insets.top), left(insets.left), right(insets.right), bottom(insets.bottom) {}

	Insets& Insets::operator=(const Insets& insets) {
		top = insets.top;
		left = insets.left;
		right = insets.right;
		bottom = insets.bottom;
		return *this;
	}

	bool Insets::operator==(const Insets& insets) const {
		return top == insets.top && left == insets.left && right == insets.right && bottom == insets.bottom;
	}

	bool Insets::operator!=(const Insets& insets) const {
		return top != insets.top || left != insets.left || right != insets.right || bottom != insets.bottom;
	}

}}}
