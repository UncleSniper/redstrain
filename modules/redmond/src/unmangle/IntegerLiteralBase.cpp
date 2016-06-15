#include "IntegerLiteralBase.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	IntegerLiteralBase::IntegerLiteralBase() {}

	IntegerLiteralBase::IntegerLiteralBase(const IntegerLiteralBase& expression) : LiteralExpression(expression) {}

	LiteralExpression::LiteralType IntegerLiteralBase::getLiteralType() const {
		return LT_INTEGER;
	}

}}}
