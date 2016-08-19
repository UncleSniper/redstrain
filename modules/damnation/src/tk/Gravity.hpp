#ifndef REDSTRAIN_MOD_DAMNATION_TK_GRAVITY_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_GRAVITY_HPP

#include "Alignment.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	enum REDSTRAIN_DAMNATION_API Gravity {
		GRAV_NORTH,
		GRAV_NORTH_EAST,
		GRAV_EAST,
		GRAV_SOUTH_EAST,
		GRAV_SOUTH,
		GRAV_SOUTH_WEST,
		GRAV_WEST,
		GRAV_NORTH_WEST,
		GRAV_CENTER
	};

	REDSTRAIN_DAMNATION_API Alignment getHorizontalAlignmentForGravity(Gravity);
	REDSTRAIN_DAMNATION_API Alignment getVerticalAlignmentForGravity(Gravity);

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_GRAVITY_HPP */
