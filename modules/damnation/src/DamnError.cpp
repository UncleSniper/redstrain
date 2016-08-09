#include "DamnError.hpp"

namespace redengine {
namespace damnation {

	DamnError::DamnError() {}

	DamnError::DamnError(const DamnError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(DamnError)

}}
