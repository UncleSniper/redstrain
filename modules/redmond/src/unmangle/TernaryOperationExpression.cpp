#include "TernaryOperationExpression.hpp"
#include "../unmangle-utils.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	TernaryOperationExpression::TernaryOperationExpression(Expression* condition, Expression* thenBranch,
			Expression* elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

	TernaryOperationExpression::TernaryOperationExpression(const TernaryOperationExpression& expression)
			: Expression(expression) {
		UnmanglePtr<Expression> c(expression.condition->cloneExpression());
		UnmanglePtr<Expression> t(expression.thenBranch->cloneExpression());
		UnmanglePtr<Expression> e(expression.elseBranch->cloneExpression());
		condition = c.ptr;
		thenBranch = t.ptr;
		elseBranch = e.ptr;
		c.ptr = t.ptr = e.ptr = NULL;
	}

	TernaryOperationExpression::~TernaryOperationExpression() {
		delete condition;
		delete thenBranch;
		delete elseBranch;
	}

	Expression::ExpressionType TernaryOperationExpression::getExpressionType() const {
		return ET_TERNARY;
	}

	void TernaryOperationExpression::print(ostream& out, int minPrecedence,
			const CurrentTemplateArguments& arguments) const {
		int myPrecedence = static_cast<int>(PREC_TERNARY);
		if(myPrecedence < minPrecedence)
			out << '(';
		condition->print(out, myPrecedence + 1, arguments);
		out << " ? ";
		thenBranch->print(out, myPrecedence, arguments);
		out << " : ";
		elseBranch->print(out, myPrecedence, arguments);
		if(myPrecedence < minPrecedence)
			out << ')';
	}

	Expression* TernaryOperationExpression::cloneExpression() const {
		return new TernaryOperationExpression(*this);
	}

}}}
