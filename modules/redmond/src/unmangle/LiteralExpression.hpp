#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_LITERALEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_LITERALEXPRESSION_HPP

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API LiteralExpression : public Expression {

	  public:
		enum LiteralType {
			LT_INTEGER,
			LT_FLOAT,
			LT_ENUM
		};

	  public:
		LiteralExpression();
		LiteralExpression(const LiteralExpression&);

		virtual ExpressionType getExpressionType() const;
		virtual LiteralType getLiteralType() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_LITERALEXPRESSION_HPP */
