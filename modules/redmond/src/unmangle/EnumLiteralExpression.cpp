#include "Name.hpp"
#include "SymbolSink.hpp"
#include "EnumLiteralExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	EnumLiteralExpression::EnumLiteralExpression(Name* typeName, int value) : typeName(typeName), value(value) {}

	EnumLiteralExpression::EnumLiteralExpression(const EnumLiteralExpression& expression)
			: LiteralExpression(expression), typeName(expression.typeName->cloneName()), value(expression.value) {}

	EnumLiteralExpression::~EnumLiteralExpression() {
		delete typeName;
	}

	LiteralExpression::LiteralType EnumLiteralExpression::getLiteralType() const {
		return LT_ENUM;
	}

	void EnumLiteralExpression::print(SymbolSink& sink, int minPrecedence,
			const CurrentTemplateArguments& arguments) const {
		int myPrecedence = static_cast<int>(PREC_PREFIX);
		if(myPrecedence < minPrecedence)
			sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		typeName->print(sink, arguments, NULL);
		sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
		sink.putIntLiteral(static_cast<int64_t>(value));
		if(myPrecedence < minPrecedence)
			sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
	}

	Expression* EnumLiteralExpression::cloneExpression() const {
		return new EnumLiteralExpression(*this);
	}

}}}
