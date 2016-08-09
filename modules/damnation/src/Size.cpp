#include "Size.hpp"

namespace redengine {
namespace damnation {

	Size::Size(unsigned width, unsigned height) : width(width), height(height) {}

	Size::Size(const Size& size) : width(size.width), height(size.height) {}

	Size& Size::operator=(const Size& size) {
		width = size.width;
		height = size.height;
		return *this;
	}

	bool Size::operator==(const Size& size) const {
		return width == size.width && height == size.height;
	}

	bool Size::operator!=(const Size& size) const {
		return width != size.width && height != size.height;
	}

}}
