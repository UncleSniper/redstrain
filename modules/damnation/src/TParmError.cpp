#include "TParmError.hpp"

namespace redengine {
namespace damnation {

	TParmError::TParmError() {}

	TParmError::TParmError(const TParmError& error) : Error(error), DamnError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(TParmError)

}}
