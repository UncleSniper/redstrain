#include "SymbolSink.hpp"
#include "UnaryOperationExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	UnaryOperationExpression::UnaryOperationExpression(Operator oper, Expression* operand)
			: oper(oper), operand(operand) {}

	UnaryOperationExpression::UnaryOperationExpression(const UnaryOperationExpression& expression)
			: Expression(expression), oper(expression.oper), operand(expression.operand->cloneExpression()) {}

	UnaryOperationExpression::~UnaryOperationExpression() {
		delete operand;
	}

	Expression::ExpressionType UnaryOperationExpression::getExpressionType() const {
		return ET_UNARY;
	}

	void UnaryOperationExpression::print(SymbolSink& sink, int minPrecedence,
			const CurrentTemplateArguments& arguments) const {
		int myPrecedence = static_cast<int>(UnaryOperationExpression::getPrecedenceOf(oper));
		if(myPrecedence < minPrecedence)
			sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		if(oper == OP_SIZEOF_EXPRESSION) {
			sink.putReserved(SymbolSink::RSV_SIZEOF);
			sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
			operand->print(sink, 0, arguments);
			sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
		}
		else {
			sink.putOperatorSymbol(*this);
			operand->print(sink, myPrecedence, arguments);
		}
		if(myPrecedence < minPrecedence)
			sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
	}

	Expression* UnaryOperationExpression::cloneExpression() const {
		return new UnaryOperationExpression(*this);
	}

	Expression::Precedence UnaryOperationExpression::getPrecedenceOf(Operator oper) {
		switch(oper) {
			case OP_POSITIVE:
			case OP_NEGATIVE:
			case OP_ADDRESS_OF:
			case OP_DEREFERENCE:
			case OP_COMPLEMENT:
			case OP_NOT:
			case OP_INCREMENT:
			case OP_DECREMENT:
			case OP_SIZEOF_EXPRESSION:
				return PREC_PREFIX;
			default:
				return PREC_PRIMARY;
		}
	}

}}}
