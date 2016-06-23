#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	Expression::Expression() {}

	Expression::Expression(const Expression&) {}

	Expression::~Expression() {}

	void Expression::print(SymbolSink& sink, const CurrentTemplateArguments& arguments) const {
		print(sink, 0, arguments);
	}

}}}
