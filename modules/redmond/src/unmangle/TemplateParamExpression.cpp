#include "TemplateArgument.hpp"
#include "TemplateParamExpression.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateParamExpression::TemplateParamExpression(unsigned parameter) : parameter(parameter) {}

	TemplateParamExpression::TemplateParamExpression(const TemplateParamExpression& expression)
			: Expression(expression), parameter(expression.parameter) {}

	Expression::ExpressionType TemplateParamExpression::getExpressionType() const {
		return ET_TEMPLATE_PARAM;
	}

	void TemplateParamExpression::print(ostream& out, int, const CurrentTemplateArguments& arguments) const {
		if(parameter >= static_cast<unsigned>(arguments.size()))
			out << '$' << parameter;
		else {
			bool lastWasGreater = false;
			arguments[parameter]->print(out, lastWasGreater, arguments);
		}
	}

	Expression* TemplateParamExpression::cloneExpression() const {
		return new TemplateParamExpression(*this);
	}

}}}
