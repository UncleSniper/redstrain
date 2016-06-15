#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMLITERALEXPRESSION_CPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMLITERALEXPRESSION_CPP

#include "Name.hpp"
#include "EnumLiteralExpression.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	EnumLiteralExpression::EnumLiteralExpression(Name* typeName, int value) : typeName(typeName), value(value) {}

	EnumLiteralExpression::EnumLiteralExpression(const EnumLiteralExpression& expression)
			: LiteralExpression(expression), typeName(expression.typeName->cloneName()), value(expression.value) {}

	EnumLiteralExpression::~EnumLiteralExpression() {
		delete typeName;
	}

	LiteralExpression::LiteralType EnumLiteralExpression::getLiteralType() const {
		return LT_ENUM;
	}

	void EnumLiteralExpression::print(ostream& out, int minPrecedence) const {
		int myPrecedence = static_cast<int>(PREC_PREFIX);
		if(myPrecedence < minPrecedence)
			out << '(';
		out << '(';
		bool lastWasGreater = false;
		typeName->print(out, lastWasGreater, NULL);
		out << ')' << value;
		if(myPrecedence < minPrecedence)
			out << ')';
	}

	Expression* EnumLiteralExpression::cloneExpression() const {
		return new EnumLiteralExpression(*this);
	}

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMLITERALEXPRESSION_CPP */
