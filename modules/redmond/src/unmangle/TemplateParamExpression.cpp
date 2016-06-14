#include "TemplateParamExpression.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateParamExpression::TemplateParamExpression(unsigned parameter) : parameter(parameter) {}

	TemplateParamExpression::TemplateParamExpression(const TemplateParamExpression& expression)
			: Expression(expression), parameter(parameter) {}

	Expression::ExpressionType TemplateParamExpression::getExpressionType() const {
		return ET_TEMPLATE_PARAM;
	}

	void TemplateParamExpression::print(ostream& out, int) const {
		out << '$' << parameter;
	}

	Expression* TemplateParamExpression::cloneExpression() const {
		return new TemplateParamExpression(*this);
	}

}}}
