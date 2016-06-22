#include "Type.hpp"
#include "SizeOfTypeExpression.hpp"

using std::ostream;

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

	void SizeOfTypeExpression::print(ostream& out, int, const CurrentTemplateArguments& arguments) const {
		out << "sizeof(";
		bool lastWasGreater = false;
		type->print(out, lastWasGreater, arguments);
		out << ')';
	}

	Expression* SizeOfTypeExpression::cloneExpression() const {
		return new SizeOfTypeExpression(*this);
	}

}}}
