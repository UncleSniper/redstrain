#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "BoxPanel.hpp"

using redengine::util::StringUtils;
using redengine::error::ProgrammingError;

namespace redengine {
namespace damnation {
namespace tk {

	BoxPanel::BoxPanel(Axis direction, const char* className)
			: SimplePanel(className ? className : "redengine.damnation.tk.BoxPanel"), direction(direction),
			alignment(ALGN_ORIGIN), excessAlignment(ALGN_ORIGIN) {}

	BoxPanel::BoxPanel(const BoxPanel& panel) : Widget(panel), SimplePanel(panel),
			direction(panel.direction), alignment(panel.alignment), excessAlignment(panel.excessAlignment) {}

	Size BoxPanel::getMinimumSizeWithinBorder() {
		Size size(0u, 0u);
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Size min((*begin)->getMinimumSize());
			if(direction == AXS_HORIZONTAL) {
				size.width += min.width;
				if(min.height > size.height)
					size.height = min.height;
			}
			else {
				if(min.width > size.width)
					size.width = min.width;
				size.height += min.height;
			}
		}
		return size;
	}

	Size BoxPanel::getMaximumSizeWithinBorder() {
		Size size = direction == AXS_HORIZONTAL ? Size(0u, Widget::NO_SIZE_LIMIT) : Size(Widget::NO_SIZE_LIMIT, 0u);
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Size max((*begin)->getMaximumSize());
			if(direction == AXS_HORIZONTAL) {
				if(max.width == Widget::NO_SIZE_LIMIT)
					size.width = Widget::NO_SIZE_LIMIT;
				else if(size.width != Widget::NO_SIZE_LIMIT)
					size.width += max.width;
				if(max.height != Widget::NO_SIZE_LIMIT
						&& (size.height == Widget::NO_SIZE_LIMIT || max.height < size.height))
					size.height = max.height;
			}
			else {
				if(max.width != Widget::NO_SIZE_LIMIT
						&& (size.width == Widget::NO_SIZE_LIMIT || max.width < size.width))
					size.width = max.width;
				if(max.height == Widget::NO_SIZE_LIMIT)
					size.height = Widget::NO_SIZE_LIMIT;
				else if(size.height != Widget::NO_SIZE_LIMIT)
					size.height += max.height;
			}
		}
		return size;
	}

	Size BoxPanel::getPreferredSizeWithinBorder() {
		Size size(0u, 0u);
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Size pref((*begin)->getPreferredSize());
			if(direction == AXS_HORIZONTAL) {
				size.width += pref.width;
				if(pref.height > size.height)
					size.height = pref.height;
			}
			else {
				if(pref.width > size.width)
					size.width = pref.width;
				size.height += pref.height;
			}
		}
		return size;
	}

	unsigned BoxPanel::getHeightForWidthWithinBorder(unsigned width) {
		unsigned height = 0u;
		Children::const_iterator begin(children.begin()), end(children.end());
		if(direction == AXS_VERTICAL) {
			for(; begin != end; ++begin)
				height += (*begin)->getHeightForWidth(width);
			return height;
		}
		unsigned minWidth = 0u, prefWidth = 0u;
		for(; begin != end; ++begin) {
			minWidth += (*begin)->getMinimumSize().width;
			prefWidth += (*begin)->getPreferredSize().width;
		}
		double scale;
		if(prefWidth <= width || minWidth <= width)
			scale = 1.0;
		else
			scale = static_cast<double>(width) / static_cast<double>(minWidth);
		begin = children.begin();
		end = children.end();
		for(; begin != end; ++begin) {
			Widget& child = **begin;
			if(prefWidth <= width)
				height += child.getHeightForWidth(child.getPreferredSize().width);
			else if(minWidth <= width)
				height += child.getHeightForWidth(child.getMinimumSize().width);
			else
				height += child.getHeightForWidth(static_cast<unsigned>(
						static_cast<double>(child.getMinimumSize().width) * scale));
		}
		return height;
	}

	unsigned BoxPanel::getWidthForHeightWithinBorder(unsigned height) {
		unsigned width = 0u;
		Children::const_iterator begin(children.begin()), end(children.end());
		if(direction == AXS_HORIZONTAL) {
			for(; begin != end; ++begin)
				height += (*begin)->getWidthForHeight(height);
			return width;
		}
		unsigned minHeight = 0u, prefHeight = 0u;
		for(; begin != end; ++begin) {
			minHeight += (*begin)->getMinimumSize().height;
			prefHeight += (*begin)->getPreferredSize().height;
		}
		double scale;
		if(prefHeight <= height || minHeight <= height)
			scale = 1.0;
		else
			scale = static_cast<double>(height) / static_cast<double>(minHeight);
		begin = children.begin();
		end = children.end();
		for(; begin != end; ++begin) {
			Widget& child = **begin;
			if(prefHeight <= height)
				width += child.getWidthForHeight(child.getPreferredSize().height);
			else if(minHeight <= height)
				width += child.getWidthForHeight(child.getMinimumSize().height);
			else
				width += child.getWidthForHeight(static_cast<unsigned>(
						static_cast<double>(child.getMinimumSize().height) * scale));
		}
		return width;
	}

	void BoxPanel::pack() {
		const Size containerSize(getSizeWithinBorder());
		Children::const_iterator begin(children.begin()), end(children.end());
		unsigned minSize = 0u, prefSize = 0u;
		for(; begin != end; ++begin) {
			Widget& child = **begin;
			Size size(child.getMinimumSize());
			minSize += direction == AXS_HORIZONTAL ? size.width : size.height;
			size = child.getPreferredSize();
			prefSize += direction == AXS_HORIZONTAL ? size.width : size.height;
		}
		unsigned available = direction == AXS_HORIZONTAL ? containerSize.width : containerSize.height;
		double scale;
		unsigned excess;
		if(prefSize <= available) {
			scale = 1.0;
			excess = available - prefSize;
		}
		else if(minSize <= available) {
			scale = 1.0;
			excess = available - minSize;
		}
		else {
			scale = static_cast<double>(available) / static_cast<double>(minSize);
			excess = 0u;
		}
		unsigned progress, single, orthogonal, offset;
		switch(excessAlignment) {
			case ALGN_ORIGIN:
				progress = 0u;
				break;
			case ALGN_CENTER:
				progress = excess / 2u;
				break;
			case ALGN_OPPOSITE:
				progress = excess;
				break;
			default:
				throw ProgrammingError("Unrecognized Alignment in BoxPanel::pack(): "
						+ StringUtils::toString(static_cast<int>(excessAlignment)));
		}
		begin = children.begin();
		end = children.end();
		for(; begin != end; ++begin) {
			Widget& child = **begin;
			if(direction == AXS_HORIZONTAL) {
				if(prefSize <= available) {
					const Size size(child.getPreferredSize());
					single = size.width;
					orthogonal = size.height;
				}
				else if(minSize <= available) {
					const Size size(child.getMinimumSize());
					single = size.width;
					orthogonal = size.height;
				}
				else {
					const Size size(child.getMinimumSize());
					single = static_cast<unsigned>(static_cast<double>(size.width) * scale);
					orthogonal = size.height;
				}
				if(orthogonal > containerSize.height)
					orthogonal = containerSize.height;
				switch(alignment) {
					case ALGN_ORIGIN:
						offset = 0u;
						break;
					case ALGN_CENTER:
						offset = (containerSize.height - orthogonal) / 2u;
						break;
					case ALGN_OPPOSITE:
						offset = containerSize.height - orthogonal;
						break;
					default:
						throw ProgrammingError("Unrecognized Alignment in BoxPanel::pack(): "
								+ StringUtils::toString(static_cast<int>(alignment)));
				}
				child.setPosition(Point(static_cast<int>(offset), static_cast<int>(progress)));
				child.setSize(Size(single, orthogonal));
			}
			else {
				if(prefSize <= available) {
					const Size size(child.getPreferredSize());
					single = size.height;
					orthogonal = size.width;
				}
				else if(minSize <= available) {
					const Size size(child.getMinimumSize());
					single = size.height;
					orthogonal = size.width;
				}
				else {
					const Size size(child.getMinimumSize());
					single = static_cast<unsigned>(static_cast<double>(size.height) * scale);
					orthogonal = size.width;
				}
				if(orthogonal > containerSize.width)
					orthogonal = containerSize.width;
				switch(alignment) {
					case ALGN_ORIGIN:
						offset = 0u;
						break;
					case ALGN_CENTER:
						offset = (containerSize.width - orthogonal) / 2u;
						break;
					case ALGN_OPPOSITE:
						offset = containerSize.width - orthogonal;
						break;
					default:
						throw ProgrammingError("Unrecognized Alignment in BoxPanel::pack(): "
								+ StringUtils::toString(static_cast<int>(alignment)));
				}
				child.setPosition(Point(static_cast<int>(progress), static_cast<int>(offset)));
				child.setSize(Size(orthogonal, single));
			}
			progress += single;
			child.pack();
		}
	}

}}}
