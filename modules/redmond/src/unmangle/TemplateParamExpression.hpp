#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMEXPRESSION_HPP

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API TemplateParamExpression : public Expression {

	  private:
		unsigned parameter;

	  public:
		TemplateParamExpression(unsigned);
		TemplateParamExpression(const TemplateParamExpression&);

		virtual ExpressionType getExpressionType() const;
		virtual void print(std::ostream&, int) const;
		virtual Expression* cloneExpression() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMEXPRESSION_HPP */
