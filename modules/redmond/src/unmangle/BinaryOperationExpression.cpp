#include "SymbolSink.hpp"
#include "BinaryOperationExpression.hpp"
#include "../unmangle-utils.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	BinaryOperationExpression::BinaryOperationExpression(Operator oper, Expression* leftOperand,
			Expression* rightOperand) : oper(oper), leftOperand(leftOperand), rightOperand(rightOperand) {}

	BinaryOperationExpression::BinaryOperationExpression(const BinaryOperationExpression& expression)
			: Expression(expression), oper(expression.oper) {
		UnmanglePtr<Expression> lo(expression.leftOperand->cloneExpression());
		UnmanglePtr<Expression> ro(expression.rightOperand->cloneExpression());
		leftOperand = lo.ptr;
		rightOperand = ro.ptr;
		lo.ptr = ro.ptr = NULL;
	}

	BinaryOperationExpression::~BinaryOperationExpression() {
		delete leftOperand;
		delete rightOperand;
	}

	Expression::ExpressionType BinaryOperationExpression::getExpressionType() const {
		return ET_BINARY;
	}

	void BinaryOperationExpression::print(SymbolSink& sink, int minPrecedence,
			const CurrentTemplateArguments& arguments) const {
		int myPrecedence = static_cast<int>(BinaryOperationExpression::getPrecedenceOf(oper));
		bool ra = BinaryOperationExpression::isRightAssociative(oper);
		if(myPrecedence < minPrecedence)
			sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		leftOperand->print(sink, ra ? myPrecedence + 1 : myPrecedence, arguments);
		sink.putOperatorSymbol(*this);
		rightOperand->print(sink, ra ? myPrecedence : myPrecedence + 1, arguments);
		if(myPrecedence < minPrecedence)
			sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
	}

	Expression* BinaryOperationExpression::cloneExpression() const {
		return new BinaryOperationExpression(*this);
	}

	Expression::Precedence BinaryOperationExpression::getPrecedenceOf(Operator oper) {
		switch(oper) {
			case OP_PLUS:
			case OP_MINUS:
				return PREC_SUM;
			case OP_MULTIPLY:
			case OP_DIVIDE:
			case OP_REMAINDER:
				return PREC_PRODUCT;
			case OP_AND:
				return PREC_BITWISE_AND;
			case OP_OR:
				return PREC_BITWISE_OR;
			case OP_XOR:
				return PREC_BITWISE_XOR;
			case OP_ASSIGN:
			case OP_PLUS_ASSIGN:
			case OP_MINUS_ASSIGN:
			case OP_MULTIPLY_ASSIGN:
			case OP_DIVIDE_ASSIGN:
			case OP_REMAINDER_ASSIGN:
			case OP_AND_ASSIGN:
			case OP_OR_ASSIGN:
			case OP_XOR_ASSIGN:
			case OP_LEFT_SHIFT_ASSIGN:
			case OP_RIGHT_SHIFT_ASSIGN:
				return PREC_ASSIGN;
			case OP_LEFT_SHIFT:
			case OP_RIGHT_SHIFT:
				return PREC_SHIFT;
			case OP_EQUAL:
			case OP_UNEQUAL:
				return PREC_EQUAL;
			case OP_LESS:
			case OP_GREATER:
			case OP_LESS_EQUAL:
			case OP_GREATER_EQUAL:
				return PREC_UNEQUAL;
			case OP_LOGICAL_AND:
				return PREC_LOGICAL_AND;
			case OP_LOGICAL_OR:
				return PREC_LOGICAL_OR;
			case OP_COMMA:
				return PREC_COMMA;
			default:
				return PREC_PRIMARY;
		}
	}

	bool BinaryOperationExpression::isRightAssociative(Operator oper) {
		switch(oper) {
			case OP_ASSIGN:
			case OP_PLUS_ASSIGN:
			case OP_MINUS_ASSIGN:
			case OP_MULTIPLY_ASSIGN:
			case OP_DIVIDE_ASSIGN:
			case OP_REMAINDER_ASSIGN:
			case OP_AND_ASSIGN:
			case OP_OR_ASSIGN:
			case OP_XOR_ASSIGN:
			case OP_LEFT_SHIFT_ASSIGN:
			case OP_RIGHT_SHIFT_ASSIGN:
				return true;
			default:
				return false;
		}
	}

}}}
