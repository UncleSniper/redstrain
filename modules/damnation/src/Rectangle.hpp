#ifndef REDSTRAIN_MOD_DAMNATION_RECTANGLE_HPP
#define REDSTRAIN_MOD_DAMNATION_RECTANGLE_HPP

#include "api.hpp"

namespace redengine {
namespace damnation {

	class Size;
	class Point;

	class REDSTRAIN_DAMNATION_API Rectangle {

	  public:
		int row, column;
		unsigned width, height;

	  public:
		Rectangle(int, int, unsigned, unsigned);
		Rectangle(const Point&, unsigned, unsigned);
		Rectangle(int, int, const Size&);
		Rectangle(const Point&, const Size&);
		Rectangle(const Rectangle&);

		Rectangle& operator=(const Rectangle&);
		bool operator==(const Rectangle&) const;
		bool operator!=(const Rectangle&) const;
		bool operator==(const Size&) const;
		bool operator!=(const Size&) const;
		Rectangle operator+(const Point&) const;
		Rectangle operator-(const Point&) const;

		void constrainInto(const Size&);
		void constrainBy(const Point&);
		Rectangle intersect(const Rectangle&) const;
		bool contains(const Point&) const;
		bool contains(const Rectangle&) const;

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_RECTANGLE_HPP */
