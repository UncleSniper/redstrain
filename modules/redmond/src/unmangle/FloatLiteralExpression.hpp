#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALEXPRESSION_HPP

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

		virtual void print(std::ostream& out, int, const CurrentTemplateArguments&) const {
			out << value;
		}

		virtual Expression* cloneExpression() const {
			return new FloatLiteralExpression(*this);
		}

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALEXPRESSION_HPP */
