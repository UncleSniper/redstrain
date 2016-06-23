#include "Type.hpp"
#include "SymbolSink.hpp"
#include "SizeOfTypeExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	SizeOfTypeExpression::SizeOfTypeExpression(Type* type) : type(type) {}

	SizeOfTypeExpression::SizeOfTypeExpression(const SizeOfTypeExpression& expression)
			: Expression(expression), type(expression.type->cloneType()) {}

	SizeOfTypeExpression::~SizeOfTypeExpression() {
		delete type;
	}

	Expression::ExpressionType SizeOfTypeExpression::getExpressionType() const {
		return ET_SIZEOF_TYPE;
	}

	void SizeOfTypeExpression::print(SymbolSink& sink, int, const CurrentTemplateArguments& arguments) const {
		sink.putReserved(SymbolSink::RSV_SIZEOF);
		sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		type->print(sink, arguments);
		sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
	}

	Expression* SizeOfTypeExpression::cloneExpression() const {
		return new SizeOfTypeExpression(*this);
	}

}}}
