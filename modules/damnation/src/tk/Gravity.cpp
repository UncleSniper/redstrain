#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "Gravity.hpp"

using redengine::util::StringUtils;
using redengine::error::ProgrammingError;

namespace redengine {
namespace damnation {
namespace tk {

	REDSTRAIN_DAMNATION_API Alignment getHorizontalAlignmentForGravity(Gravity gravity) {
		switch(gravity) {
#define clamp(g, a) \
	case g: \
		return a;
			clamp(GRAV_NORTH, ALGN_CENTER)
			clamp(GRAV_NORTH_EAST, ALGN_OPPOSITE)
			clamp(GRAV_EAST, ALGN_OPPOSITE)
			clamp(GRAV_SOUTH_EAST, ALGN_OPPOSITE)
			clamp(GRAV_SOUTH, ALGN_CENTER)
			clamp(GRAV_SOUTH_WEST, ALGN_ORIGIN)
			clamp(GRAV_WEST, ALGN_ORIGIN)
			clamp(GRAV_NORTH_WEST, ALGN_ORIGIN)
			clamp(GRAV_CENTER, ALGN_CENTER)
#undef clamp
			default:
				throw ProgrammingError("Unrecognized Gravity in getHorizontalAlignmentForGravity(): "
						+ StringUtils::toString(static_cast<int>(gravity)));
		}
	}

	REDSTRAIN_DAMNATION_API Alignment getVerticalAlignmentForGravity(Gravity gravity) {
		switch(gravity) {
#define clamp(g, a) \
	case g: \
		return a;
			clamp(GRAV_NORTH, ALGN_ORIGIN)
			clamp(GRAV_NORTH_EAST, ALGN_ORIGIN)
			clamp(GRAV_EAST, ALGN_CENTER)
			clamp(GRAV_SOUTH_EAST, ALGN_OPPOSITE)
			clamp(GRAV_SOUTH, ALGN_OPPOSITE)
			clamp(GRAV_SOUTH_WEST, ALGN_OPPOSITE)
			clamp(GRAV_WEST, ALGN_CENTER)
			clamp(GRAV_NORTH_WEST, ALGN_ORIGIN)
			clamp(GRAV_CENTER, ALGN_CENTER)
#undef clamp
			default:
				throw ProgrammingError("Unrecognized Gravity in getHorizontalAlignmentForGravity(): "
						+ StringUtils::toString(static_cast<int>(gravity)));
		}
	}

}}}
