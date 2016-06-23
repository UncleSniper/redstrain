#include "Type.hpp"
#include "SymbolSink.hpp"
#include "CastExpression.hpp"
#include "../unmangle-utils.hpp"

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

	void CastExpression::print(SymbolSink& sink, int minPrecedence,
			const CurrentTemplateArguments& arguments) const {
		int myPrecedence = static_cast<int>(PREC_PREFIX);
		if(myPrecedence < minPrecedence)
			sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		type->print(sink, arguments);
		sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
		operand->print(sink, myPrecedence, arguments);
		if(myPrecedence < minPrecedence)
			sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
	}

	Expression* CastExpression::cloneExpression() const {
		return new CastExpression(*this);
	}

}}}
