#include "UnaryOperationExpression.hpp"

using std::ostream;

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

	void UnaryOperationExpression::print(ostream& out, int minPrecedence) const {
		int myPrecedence = static_cast<int>(UnaryOperationExpression::getPrecedenceOf(oper));
		if(myPrecedence < minPrecedence)
			out << '(';
		switch(oper) {
			case OP_POSITIVE:
				out << '+';
				operand->print(out, myPrecedence);
				break;
			case OP_NEGATIVE:
				out << '-';
				operand->print(out, myPrecedence);
				break;
			case OP_ADDRESS_OF:
				out << '&';
				operand->print(out, myPrecedence);
				break;
			case OP_DEREFERENCE:
				out << '*';
				operand->print(out, myPrecedence);
				break;
			case OP_COMPLEMENT:
				out << '~';
				operand->print(out, myPrecedence);
				break;
			case OP_NOT:
				out << '!';
				operand->print(out, myPrecedence);
				break;
			case OP_INCREMENT:
				out << "++";
				operand->print(out, myPrecedence);
				break;
			case OP_DECREMENT:
				out << "--";
				operand->print(out, myPrecedence);
				break;
			case OP_SIZEOF_EXPRESSION:
				out << "sizeof(";
				operand->print(out, 0);
				out << ')';
				break;
			default:
				out << "<unknown unary operator> ";
				operand->print(out, PREC_PRIMARY);
				break;
		}
		if(myPrecedence < minPrecedence)
			out << ')';
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
