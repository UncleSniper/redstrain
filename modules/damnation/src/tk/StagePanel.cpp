#include "StagePanel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	StagePanel::StagePanel(const char* className)
			: ConstraintPanel<StageConstraints>(className ? className : "redengine.damnation.tk.StagePanel") {}

	StagePanel::StagePanel(const StagePanel& panel) : Widget(panel), ConstraintPanel<StageConstraints>(panel) {}

	Size StagePanel::getMinimumSizeWithinBorder() {
		Size size(0u, 0u);
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Child& c = **begin;
			const Size min(c.getWidget()->getMinimumSize());
			const Size enclosing(c.getConstraint().getEnclosingSize(min, true));
			if(enclosing.width > size.width)
				size.width = enclosing.width;
			if(enclosing.height > size.height)
				size.height = enclosing.height;
		}
		return size;
	}

	Size StagePanel::getMaximumSizeWithinBorder() {
		Size size(Widget::NO_SIZE_LIMIT, Widget::NO_SIZE_LIMIT);
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Child& c = **begin;
			const Size max(c.getWidget()->getMaximumSize());
			Size enclosing(c.getConstraint().getEnclosingSize(max, false));
			if(max.width == Widget::NO_SIZE_LIMIT)
				enclosing.width = Widget::NO_SIZE_LIMIT;
			if(max.height ==  Widget::NO_SIZE_LIMIT)
				enclosing.height = Widget::NO_SIZE_LIMIT;
			if(max.width != Widget::NO_SIZE_LIMIT
					&& (size.width == Widget::NO_SIZE_LIMIT || enclosing.width < size.width))
				size.width = enclosing.width;
			if(max.height != Widget::NO_SIZE_LIMIT
					&& (size.height == Widget::NO_SIZE_LIMIT || enclosing.height < size.height))
				size.height = enclosing.height;
		}
		return size;
	}

	Size StagePanel::getPreferredSizeWithinBorder() {
		Size size(0u, 0u);
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Child& c = **begin;
			const Size pref(c.getWidget()->getPreferredSize());
			const Size enclosing(c.getConstraint().getEnclosingSize(pref, false));
			if(enclosing.width > size.width)
				size.width = enclosing.width;
			if(enclosing.height > size.height)
				size.height = enclosing.height;
		}
		return size;
	}

	unsigned StagePanel::getHeightForWidthWithinBorder(unsigned width) {
		unsigned height = 0u;
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Child& c = **begin;
			unsigned ch = c.getWidget()->getHeightForWidth(width);
			const Size enclosing(c.getConstraint().getEnclosingSize(Size(width, ch), false));
			if(enclosing.height > height)
				height = enclosing.height;
		}
		return height;
	}

	unsigned StagePanel::getWidthForHeightWithinBorder(unsigned height) {
		unsigned width = 0u;
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Child& c = **begin;
			unsigned cw = c.getWidget()->getWidthForHeight(height);
			const Size enclosing(c.getConstraint().getEnclosingSize(Size(cw, height), false));
			if(enclosing.width > width)
				width = enclosing.width;
		}
		return width;
	}

	void StagePanel::pack() {
		const Size containerSize(getSizeWithinBorder());
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			const Child& c = **begin;
			const Rectangle rectangle(c.getConstraint().getChildSize(containerSize));
			Widget& w = *c.getWidget();
			w.setPosition(Point(rectangle.row, rectangle.column));
			w.setSize(Size(rectangle.width, rectangle.height));
			w.pack();
		}
	}

}}}
