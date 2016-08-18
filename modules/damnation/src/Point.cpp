#include "Point.hpp"

namespace redengine {
namespace damnation {

	Point::Point(int row, int column) : row(row), column(column) {}

	Point::Point(const Point& point) : row(point.row), column(point.column) {}

	Point& Point::operator=(const Point& point) {
		row = point.row;
		column = point.column;
		return *this;
	}

	bool Point::operator==(const Point& point) const {
		return row == point.row && column == point.column;
	}

	bool Point::operator!=(const Point& point) const {
		return row != point.row || column != point.column;
	}

}}
