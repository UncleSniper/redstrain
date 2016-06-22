#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_BINARYOPERATIONEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_BINARYOPERATIONEXPRESSION_HPP

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API BinaryOperationExpression : public Expression {

	  public:
		enum Operator {
			OP_PLUS,
			OP_MINUS,
			OP_MULTIPLY,
			OP_DIVIDE,
			OP_REMAINDER,
			OP_AND,
			OP_OR,
			OP_XOR,
			OP_ASSIGN,
			OP_PLUS_ASSIGN,
			OP_MINUS_ASSIGN,
			OP_MULTIPLY_ASSIGN,
			OP_DIVIDE_ASSIGN,
			OP_REMAINDER_ASSIGN,
			OP_AND_ASSIGN,
			OP_OR_ASSIGN,
			OP_XOR_ASSIGN,
			OP_LEFT_SHIFT,
			OP_RIGHT_SHIFT,
			OP_LEFT_SHIFT_ASSIGN,
			OP_RIGHT_SHIFT_ASSIGN,
			OP_EQUAL,
			OP_UNEQUAL,
			OP_LESS,
			OP_GREATER,
			OP_LESS_EQUAL,
			OP_GREATER_EQUAL,
			OP_LOGICAL_AND,
			OP_LOGICAL_OR,
			OP_COMMA
		};

	  private:
		Operator oper;
		Expression *leftOperand, *rightOperand;

	  public:
		BinaryOperationExpression(Operator, Expression*, Expression*);
		BinaryOperationExpression(const BinaryOperationExpression&);
		virtual ~BinaryOperationExpression();

		inline Operator getOperator() const {
			return oper;
		}

		inline Expression& getLeftOperand() const {
			return *leftOperand;
		}

		inline Expression& getRightOperand() const {
			return *rightOperand;
		}

		virtual ExpressionType getExpressionType() const;
		virtual void print(std::ostream&, int, const CurrentTemplateArguments&) const;
		virtual Expression* cloneExpression() const;

		static Precedence getPrecedenceOf(Operator);
		static bool isRightAssociative(Operator);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_BINARYOPERATIONEXPRESSION_HPP */
