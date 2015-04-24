#include "IOError.hpp"

namespace redengine {
namespace error {

	IOError::IOError() {}

	IOError::IOError(const IOError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(IOError)

}}
