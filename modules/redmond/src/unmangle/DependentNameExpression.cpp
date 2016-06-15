#include "Type.hpp"
#include "UnqualifiedName.hpp"
#include "DependentNameExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	DependentNameExpression::DependentNameExpression(Type* type, UnqualifiedName* name) : type(type), name(name) {}

	DependentNameExpression::DependentNameExpression(const DependentNameExpression& expression)
			: Expression(expression), type(expression.type), name(expression.name) {
		UnmangleCollectionPtr<Arguments> delArgs(&arguments);
		ArgumentIterator begin(segment.arguments.begin()), end(segment.arguments.end());
		UnmanglePtr<TemplateArgument> arg(NULL);
		for(; begin != end; ++begin) {
			arg.ptr = (*begin)->cloneTemplateArgument();
			arguments.push_back(arg.ptr);
			arg.ptr = NULL;
		}
		delArgs.ptr = NULL;
	}

	DependentNameExpression::~DependentNameExpression() {
		delete type;
		delete name;
		ArgumentIterator begin(arguments.begin()), end(arguments.end());
		for(; begin != end; ++begin)
			delete *begin;
	}

	bool DependentNameExpression::hasArguments() const {
		return !arguments.empty();
	}

	unsigned DependentNameExpression::getArgumentCount() const {
		return static_cast<unsigned>(arguments.size());
	}

	void DependentNameExpression::getArguments(ArgumentIterator& begin, ArgumentIterator& end) const {
		begin = arguments.begin();
		end = arguments.end();
	}

	void DependentNameExpression::addArgument(TemplateArgument& argument) {
		arguments.push_back(&argument);
	}

	ExpressionType DependentNameExpression::getExpressionType() const {
		return ET_DEPENDENT_NAME;
	}

	void DependentNameExpression::print(ostream& out, int minPrecedence) const {
		bool lastWasGreater = false;
		type->print(out, lastWasGreater);
		out << "::";
		lastWasGreater = false;
		name->print(out, lastWasGreater);
		if(!arguments.empty()) {
			ArgumentIterator abegin(arguments.begin()), end(arguments.end());
			out << '<';
			bool first = true;
			for(; abegin != aend; ++abegin) {
				if(first)
					first = false;
				else
					out << ", ";
				lastWasGreater = false;
				(*abegin)->print(out, lastWasGreater);
			}
			if(lastWasGreater)
				out << ' ';
			out >> '>';
		}
	}

	Expression* DependentNameExpression::cloneExpression() const {
		return new DependentNameExpression(*this);
	}

}}}
