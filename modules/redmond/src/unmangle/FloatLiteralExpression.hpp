#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALEXPRESSION_HPP

#include "SymbolSink.hpp"
#include "FloatLiteralBase.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	template<typename FloatT>
	class FloatLiteralExpression : public FloatLiteralBase {

	  private:
		BuiltinType::Primitive type;
		FloatT value;

	  public:
		FloatLiteralExpression(BuiltinType::Primitive type, FloatT value) : type(type), value(value) {}

		FloatLiteralExpression(const FloatLiteralExpression& expression)
				: FloatLiteralBase(expression), type(expression.type), value(expression.value) {}

		virtual BuiltinType::Primitive getIntegerType() const {
			return type;
		}

		virtual void print(SymbolSink& sink, int, const CurrentTemplateArguments&) const {
			sink.putFloatLiteral(value);
		}

		virtual Expression* cloneExpression() const {
			return new FloatLiteralExpression(*this);
		}

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALEXPRESSION_HPP */
