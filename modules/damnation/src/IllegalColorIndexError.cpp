#include "IllegalColorIndexError.hpp"

namespace redengine {
namespace damnation {

	IllegalColorIndexError::IllegalColorIndexError(unsigned index) : index(index) {}

	IllegalColorIndexError::IllegalColorIndexError(const IllegalColorIndexError& error)
			: Error(error), IllegalArgumentError(error), DamnError(error), index(error.index) {}

	REDSTRAIN_DEFINE_ERROR(IllegalColorIndexError) {
		out << "Illegal color index: " << index;
	}

}}
