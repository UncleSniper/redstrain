#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_CASTEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_CASTEXPRESSION_HPP

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Type;

	class REDSTRAIN_REDMOND_API CastExpression : public Expression {

	  private:
		Type* type;
		Expression* operand;

	  protected:
		virtual void print(std::ostream&, int) const;

	  public:
		CastExpression(Type*, Expression*);
		CastExpression(const CastExpression&);
		virtual ~CastExpression();

		inline Type& getType() const {
			return *type;
		}

		inline Expression& getOperand() const {
			return *operand;
		}

		virtual ExpressionType getExpressionType() const;
		virtual Expression* cloneExpression() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_CASTEXPRESSION_HPP */
