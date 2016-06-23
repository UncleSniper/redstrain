#include "SymbolSink.hpp"
#include "TemplateArgument.hpp"
#include "TemplateParamExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateParamExpression::TemplateParamExpression(unsigned parameter) : parameter(parameter) {}

	TemplateParamExpression::TemplateParamExpression(const TemplateParamExpression& expression)
			: Expression(expression), parameter(expression.parameter) {}

	Expression::ExpressionType TemplateParamExpression::getExpressionType() const {
		return ET_TEMPLATE_PARAM;
	}

	void TemplateParamExpression::print(SymbolSink& sink, int, const CurrentTemplateArguments& arguments) const {
		if(parameter >= static_cast<unsigned>(arguments.size()))
			sink.putUndefinedTemplateParameter(parameter);
		else
			arguments[parameter]->print(sink, arguments);
	}

	Expression* TemplateParamExpression::cloneExpression() const {
		return new TemplateParamExpression(*this);
	}

}}}
