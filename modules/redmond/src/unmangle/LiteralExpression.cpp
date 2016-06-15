#include "LiteralExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	LiteralExpression::LiteralExpression() {}

	LiteralExpression::LiteralExpression(const LiteralExpression& expression) : Expression(expression) {}

	Expression::ExpressionType LiteralExpression::getExpressionType() const {
		return ET_LITERAL;
	}

}}}
