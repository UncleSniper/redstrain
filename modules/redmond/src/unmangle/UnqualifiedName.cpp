#include "UnqualifiedName.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	UnqualifiedName::UnqualifiedName() {}

	UnqualifiedName::UnqualifiedName(const UnqualifiedName& name) : Name(name) {}

	const string* UnqualifiedName::getUnqualifiedClassNameData() const {
		return NULL;
	}

}}}
