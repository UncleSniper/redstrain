#include "BinaryOperationExpression.hpp"
#include "../unmangle-utils.hpp"

using std::ostream;

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

	void BinaryOperationExpression::print(ostream& out, int minPrecedence) const {
		int myPrecedence = static_cast<int>(BinaryOperationExpression::getPrecedenceOf(oper));
		bool ra = BinaryOperationExpression::isRightAssociative(oper);
		if(myPrecedence < minPrecedence)
			out << '(';
		const char* symbol;
		#define clamp(constant, sym) \
			case constant: \
				symbol = sym; \
				break;
		switch(oper) {
			clamp(OP_PLUS, "+")
			clamp(OP_MINUS, "-")
			clamp(OP_MULTIPLY, "*")
			clamp(OP_DIVIDE, "/")
			clamp(OP_REMAINDER, "%")
			clamp(OP_AND, "&")
			clamp(OP_OR, "|")
			clamp(OP_XOR, "^")
			clamp(OP_ASSIGN, "=")
			clamp(OP_PLUS_ASSIGN, "+=")
			clamp(OP_MINUS_ASSIGN, "-=")
			clamp(OP_MULTIPLY_ASSIGN, "*=")
			clamp(OP_DIVIDE_ASSIGN, "/=")
			clamp(OP_REMAINDER_ASSIGN, "%=")
			clamp(OP_AND_ASSIGN, "&=")
			clamp(OP_OR_ASSIGN, "|=")
			clamp(OP_XOR_ASSIGN, "^=")
			clamp(OP_LEFT_SHIFT_ASSIGN, "<<=")
			clamp(OP_RIGHT_SHIFT_ASSIGN, ">>=")
			clamp(OP_LEFT_SHIFT, "<<")
			clamp(OP_RIGHT_SHIFT, ">>")
			clamp(OP_EQUAL, "==")
			clamp(OP_UNEQUAL, "!=")
			clamp(OP_LESS, "<")
			clamp(OP_GREATER, ">")
			clamp(OP_LESS_EQUAL, "<=")
			clamp(OP_GREATER_EQUAL, ">=")
			clamp(OP_LOGICAL_AND, "&&")
			clamp(OP_LOGICAL_OR, "||")
			clamp(OP_COMMA, ",")
			default:
				symbol = "<unknown binary operator>";
				break;
		}
		leftOperand->print(out, ra ? myPrecedence + 1 : myPrecedence);
		out << ' ' << symbol << ' ';
		rightOperand->print(out, ra ? myPrecedence : myPrecedence + 1);
		if(myPrecedence < minPrecedence)
			out << ')';
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
