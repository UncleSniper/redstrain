#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	Type::Type() {}

	Type::Type(const Type&) {}

	Type::~Type() {}

	void Type::print(SymbolSink& sink, const CurrentTemplateArguments& arguments) const {
		print(sink, arguments, NULL);
	}

	bool Type::inlinesEnclosingClassName() const {
		return false;
	}

}}}
