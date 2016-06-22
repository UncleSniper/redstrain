#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_UNARYOPERATIONEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_UNARYOPERATIONEXPRESSION_HPP

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API UnaryOperationExpression : public Expression {

	  public:
		enum Operator {
			OP_POSITIVE,
			OP_NEGATIVE,
			OP_ADDRESS_OF,
			OP_DEREFERENCE,
			OP_COMPLEMENT,
			OP_NOT,
			OP_INCREMENT,
			OP_DECREMENT,
			OP_SIZEOF_EXPRESSION
		};

	  private:
		Operator oper;
		Expression* operand;

	  public:
		UnaryOperationExpression(Operator, Expression*);
		UnaryOperationExpression(const UnaryOperationExpression&);
		virtual ~UnaryOperationExpression();

		inline Operator getOperator() const {
			return oper;
		}

		inline Expression& getOperand() const {
			return *operand;
		}

		virtual ExpressionType getExpressionType() const;
		virtual void print(std::ostream&, int, const CurrentTemplateArguments&) const;
		virtual Expression* cloneExpression() const;

		static Precedence getPrecedenceOf(Operator);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_UNARYOPERATIONEXPRESSION_HPP */
