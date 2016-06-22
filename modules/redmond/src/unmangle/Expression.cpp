#include "Expression.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	Expression::Expression() {}

	Expression::Expression(const Expression&) {}

	Expression::~Expression() {}

	void Expression::print(ostream& out, const CurrentTemplateArguments& arguments) const {
		print(out, 0, arguments);
	}

}}}
