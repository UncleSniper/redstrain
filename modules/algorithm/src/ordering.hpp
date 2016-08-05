#ifndef REDSTRAIN_MOD_ALGORITHM_ORDERING_HPP
#define REDSTRAIN_MOD_ALGORITHM_ORDERING_HPP

#include <redstrain/util/OrderRelation.hpp>

namespace redengine {
namespace algorithm {

	template<typename T>
	util::OrderRelation standardCompare(T a, T b) {
		if(a < b)
			return util::OR_LESS;
		if(b < a)
			return util::OR_GREATER;
		return util::OR_EQUAL;
	}

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_ORDERING_HPP */
