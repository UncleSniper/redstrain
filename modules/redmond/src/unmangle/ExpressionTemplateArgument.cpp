#include "Expression.hpp"
#include "ExpressionTemplateArgument.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	ExpressionTemplateArgument::ExpressionTemplateArgument(Expression* expression) : expression(expression) {}

	ExpressionTemplateArgument::ExpressionTemplateArgument(const ExpressionTemplateArgument& argument)
			: TemplateArgument(argument), expression(argument.expression->cloneExpression()) {}

	ExpressionTemplateArgument::~ExpressionTemplateArgument() {
		delete expression;
	}

	TemplateArgument::ArgumentType ExpressionTemplateArgument::getArgumentType() const {
		return AT_EXPRESSION;
	}

	void ExpressionTemplateArgument::print(ostream& out, bool& lastWasGreater,
			const CurrentTemplateArguments& arguments) const {
		expression->print(out, arguments);
		lastWasGreater = false;
	}

	TemplateArgument* ExpressionTemplateArgument::cloneTemplateArgument() const {
		return new ExpressionTemplateArgument(*this);
	}

}}}
