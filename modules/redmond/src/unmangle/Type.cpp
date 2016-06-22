#include "Type.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	Type::Type() {}

	Type::Type(const Type&) {}

	Type::~Type() {}

	void Type::print(ostream& out, bool& lastWasGreater, const CurrentTemplateArguments& arguments) const {
		print(out, lastWasGreater, arguments, NULL);
	}

	bool Type::inlinesEnclosingClassName() const {
		return false;
	}

}}}
