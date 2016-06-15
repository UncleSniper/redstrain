#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TERNARYOPERATIONEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TERNARYOPERATIONEXPRESSION_HPP

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API TernaryOperationExpression : public Expression {

	  private:
		Expression *condition, *thenBranch, *elseBranch;

	  public:
		TernaryOperationExpression(Expression*, Expression*, Expression*);
		TernaryOperationExpression(const TernaryOperationExpression&);
		virtual ~TernaryOperationExpression();

		inline Expression& getCondition() const {
			return *condition;
		}

		inline Expression& getThenBranch() const {
			return *thenBranch;
		}

		inline Expression& getElseBranch() const {
			return *elseBranch;
		}

		virtual ExpressionType getExpressionType() const;
		virtual void print(std::ostream&, int) const;
		virtual Expression* cloneExpression() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TERNARYOPERATIONEXPRESSION_HPP */
