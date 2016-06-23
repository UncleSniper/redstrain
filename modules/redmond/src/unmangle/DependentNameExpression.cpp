#include "Type.hpp"
#include "SymbolSink.hpp"
#include "UnqualifiedName.hpp"
#include "TemplateArgument.hpp"
#include "DependentNameExpression.hpp"
#include "../unmangle-utils.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	DependentNameExpression::DependentNameExpression(Type* type, UnqualifiedName* name) : type(type), name(name) {}

	DependentNameExpression::DependentNameExpression(const DependentNameExpression& expression)
			: Expression(expression), type(expression.type), name(expression.name) {
		UnmangleCollectionPtr<Arguments> delArgs(&arguments);
		ArgumentIterator begin(expression.arguments.begin()), end(expression.arguments.end());
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

	Expression::ExpressionType DependentNameExpression::getExpressionType() const {
		return ET_DEPENDENT_NAME;
	}

	void DependentNameExpression::print(SymbolSink& sink, int, const CurrentTemplateArguments& targuments) const {
		type->print(sink, targuments);
		sink.putSeparator(SymbolSink::SEP_PAAMAYIM_NEKUDOTAYIM);
		name->print(sink, targuments, NULL);
		if(!arguments.empty()) {
			ArgumentIterator abegin(arguments.begin()), aend(arguments.end());
			sink.putSeparator(SymbolSink::SEP_LEFT_ANGLE);
			unsigned space = sink.getRemainingColumnCount();
			bool breakArgs = false, first = true;
			if(space) {
				unsigned iwidth = 0u;
				ArgumentIterator sabegin(abegin), saend(aend);
				for(; sabegin != saend; ++sabegin) {
					if(first)
						first = false;
					else
						iwidth += sink.getInlineWidthOf(SymbolSink::SEP_COMMA)
								+ sink.getInlineWidthOf(SymbolSink::SEP_AFTER_COMMA);
					iwidth += sink.getInlineWidthOf(**sabegin, targuments);
				}
				first = false;
				breakArgs = iwidth > space;
			}
			for(; abegin != aend; ++abegin) {
				if(first) {
					first = false;
					if(breakArgs)
						sink.startNewLine(1);
				}
				else {
					sink.putSeparator(SymbolSink::SEP_COMMA);
					if(breakArgs)
						sink.startNewLine(0);
					else
						sink.putSeparator(SymbolSink::SEP_AFTER_COMMA);
				}
				(*abegin)->print(sink, targuments);
			}
			if(breakArgs && !first)
				sink.startNewLine(-1);
			sink.putSeparator(SymbolSink::SEP_RIGHT_ANGLE);
		}
	}

	Expression* DependentNameExpression::cloneExpression() const {
		return new DependentNameExpression(*this);
	}

}}}
