#include "Size.hpp"
#include "Point.hpp"
#include "Rectangle.hpp"

namespace redengine {
namespace damnation {

	Rectangle::Rectangle(int row, int column, unsigned width, unsigned height)
			: row(row), column(column), width(width), height(height) {}

	Rectangle::Rectangle(const Point& point, unsigned width, unsigned height)
			: row(point.row), column(point.column), width(width), height(height) {}

	Rectangle::Rectangle(int row, int column, const Size& size)
			: row(row), column(column), width(size.width), height(size.height) {}

	Rectangle::Rectangle(const Point& point, const Size& size)
			: row(point.row), column(point.column), width(size.width), height(size.height) {}

	Rectangle::Rectangle(const Rectangle& rectangle)
			: row(rectangle.row), column(rectangle.column), width(rectangle.width), height(rectangle.height) {}

	Rectangle& Rectangle::operator=(const Rectangle& rectangle) {
		row = rectangle.row;
		column = rectangle.column;
		width = rectangle.width;
		height = rectangle.height;
		return *this;
	}

	bool Rectangle::operator==(const Rectangle& rectangle) const {
		return row == rectangle.row && column == rectangle.column
				&& width == rectangle.width && height == rectangle.height;
	}

	bool Rectangle::operator!=(const Rectangle& rectangle) const {
		return row != rectangle.row || column != rectangle.column
				|| width != rectangle.width || height != rectangle.height;
	}

	bool Rectangle::operator==(const Size& size) const {
		return !row && !column && width == size.width && height == size.height;
	}

	bool Rectangle::operator!=(const Size& size) const {
		return row || column || width != size.width || height != size.height;
	}

	Rectangle Rectangle::operator+(const Point& point) const {
		return Rectangle(row + point.row, column + point.column, width, height);
	}

	Rectangle Rectangle::operator-(const Point& point) const {
		return Rectangle(row - point.row, column - point.column, width, height);
	}

	void Rectangle::constrainInto(const Size& size) {
		if(row < 0)
			row = 0;
		if(static_cast<unsigned>(row) >= size.height)
			row = static_cast<int>(size.height ? size.height - 1u : 0u);
		if(column < 0)
			column = 0;
		if(static_cast<unsigned>(column) >= size.width)
			column = static_cast<int>(size.width ? size.width - 1u : 0u);
		const unsigned wbound = size.width - static_cast<unsigned>(column);
		const unsigned hbound = size.height - static_cast<unsigned>(row);
		if(width > wbound)
			width = wbound;
		if(height > hbound)
			height = hbound;
	}

	void Rectangle::constrainBy(const Point& point) {
		if(point.row > row)
			row = point.row;
		if(point.column > column)
			column = point.column;
	}

	Rectangle Rectangle::intersect(const Rectangle& rectangle) const {
		const int myBottom = row + static_cast<int>(height);
		const int myRight = column + static_cast<int>(width);
		const int theirBottom = rectangle.row + static_cast<int>(rectangle.height);
		const int theirRight = rectangle.column + static_cast<int>(rectangle.width);
		const int top = row > rectangle.row ? row : rectangle.row;
		const int bottom = myBottom < theirBottom ? myBottom : theirBottom;
		const int left = column > rectangle.column ? column : rectangle.column;
		const int right = myRight < theirRight ? myRight : theirRight;
		return Rectangle(
			top,
			left,
			right < left ? 0u : static_cast<unsigned>(right - left),
			bottom < top ? 0u : static_cast<unsigned>(bottom - top)
		);
	}

	bool Rectangle::contains(const Point& point) const {
		return point.row >= row && point.row < row + static_cast<int>(height)
				&& point.column >= column && point.column < column  + static_cast<int>(width);
	}

	bool Rectangle::contains(const Rectangle& rectangle) const {
		return rectangle.row >= row
				&& rectangle.row + static_cast<int>(rectangle.height) < row + static_cast<int>(height)
				&& rectangle.column >= column
				&& rectangle.column + static_cast<int>(rectangle.width) < column + static_cast<int>(width);
	}

}}
