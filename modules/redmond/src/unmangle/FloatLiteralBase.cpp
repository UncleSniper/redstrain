#include "FloatLiteralBase.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	FloatLiteralBase::FloatLiteralBase() {}

	FloatLiteralBase::FloatLiteralBase(const FloatLiteralBase& expression) : LiteralExpression(expression) {}

	LiteralExpression::LiteralType FloatLiteralBase::getLiteralType() const {
		return LT_FLOAT;
	}

}}}
