#include "InputConverter.hpp"

namespace redengine {
namespace damnation {
namespace unixutils {

	// ======== Stage ========

	InputConverter::Stage::Stage() {}

	InputConverter::Stage::Stage(const Stage&) {}

	InputConverter::Stage::~Stage() {}

	// ======== InputConverter ========

	InputConverter::InputConverter() {}

	InputConverter::InputConverter(const InputConverter& converter) : ReferenceCounted(converter) {}

}}}
