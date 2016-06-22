#include "CPPSymbol.hpp"
#include "ExternalNameExpression.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	ExternalNameExpression::ExternalNameExpression(CPPSymbol* symbol) : symbol(symbol) {}

	ExternalNameExpression::ExternalNameExpression(const ExternalNameExpression& expression)
			: Expression(expression), symbol(expression.symbol->cloneSymbol()) {}

	ExternalNameExpression::~ExternalNameExpression() {
		delete symbol;
	}

	Expression::ExpressionType ExternalNameExpression::getExpressionType() const {
		return ET_EXTERNAL_NAME;
	}

	void ExternalNameExpression::print(ostream& out, int, const CurrentTemplateArguments&) const {
		bool lastWasGreater = false;
		symbol->print(out, lastWasGreater);
	}

	Expression* ExternalNameExpression::cloneExpression() const {
		return new ExternalNameExpression(*this);
	}

}}}
