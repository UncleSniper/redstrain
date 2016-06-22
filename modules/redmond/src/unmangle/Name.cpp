#include "Name.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	Name::Name() {}

	Name::Name(const Name&) {}

	Name::~Name() {}

	int Name::getNameCVQualifiers() const {
		return 0;
	}

	void Name::getCurrentTemplateArguments(CurrentTemplateArguments& arguments) const {
		arguments.clear();
	}

}}}
