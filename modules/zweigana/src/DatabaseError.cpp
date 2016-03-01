#include "DatabaseError.hpp"

namespace redengine {
namespace zweigana {

	DatabaseError::DatabaseError() {}

	DatabaseError::DatabaseError(const DatabaseError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(DatabaseError)

}}
