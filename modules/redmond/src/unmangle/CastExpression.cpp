#include "Type.hpp"
#include "CastExpression.hpp"
#include "../unmangle-utils.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	CastExpression::CastExpression(Type* type, Expression* operand) : type(type), operand(operand) {}

	CastExpression::CastExpression(const CastExpression& expression) : Expression(expression) {
		UnmanglePtr<Type> t(expression.type->cloneType());
		UnmanglePtr<Expression> o(expression.operand->cloneExpression());
		type = t.ptr;
		operand = o.ptr;
		t.ptr = NULL;
		o.ptr = NULL;
	}

	CastExpression::~CastExpression() {
		delete type;
		delete operand;
	}

	Expression::ExpressionType CastExpression::getExpressionType() const {
		return ET_CAST;
	}

	void CastExpression::print(ostream& out, int minPrecedence) const {
		int myPrecedence = static_cast<int>(PREC_PREFIX);
		if(myPrecedence < minPrecedence)
			out << '(';
		out << '(';
		bool lastWasGreater = false;
		type->print(out, lastWasGreater);
		out << ')';
		operand->print(out, myPrecedence);
		if(myPrecedence < minPrecedence)
			out << ')';
	}

	Expression* CastExpression::cloneExpression() const {
		return new CastExpression(*this);
	}

}}}
