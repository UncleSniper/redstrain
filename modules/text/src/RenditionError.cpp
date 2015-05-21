#include "RenditionError.hpp"

namespace redengine {
namespace text {

	RenditionError::RenditionError() {}

	RenditionError::RenditionError(const RenditionError& error) : TextError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(RenditionError)

}}
