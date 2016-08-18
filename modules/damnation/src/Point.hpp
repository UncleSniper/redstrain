#ifndef REDSTRAIN_MOD_DAMNATION_POINT_HPP
#define REDSTRAIN_MOD_DAMNATION_POINT_HPP

#include "api.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API Point {

	  public:
		int row, column;

	  public:
		Point(int, int);
		Point(const Point&);

		Point& operator=(const Point&);
		bool operator==(const Point&) const;
		bool operator!=(const Point&) const;

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_POINT_HPP */
