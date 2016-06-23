#include "CPPSymbol.hpp"
#include "ExternalNameExpression.hpp"

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

	void ExternalNameExpression::print(SymbolSink& sink, int, const CurrentTemplateArguments&) const {
		symbol->print(sink);
	}

	Expression* ExternalNameExpression::cloneExpression() const {
		return new ExternalNameExpression(*this);
	}

}}}
