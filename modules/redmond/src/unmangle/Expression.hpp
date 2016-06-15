#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_EXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_EXPRESSION_HPP

#include <iostream>

#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API Expression {

	  public:
		enum ExpressionType {
			ET_UNARY,
			ET_BINARY,
			ET_TERNARY,
			ET_CAST,
			ET_SIZEOF_TYPE,
			ET_TEMPLATE_PARAM,
			ET_DEPENDENT_NAME,
			ET_LITERAL,
			ET_EXTERNAL_NAME
		};

		enum Precedence {
			PREC_COMMA = 0,
			PREC_ASSIGN = 1,
			PREC_TERNARY = 1,
			PREC_LOGICAL_OR = 2,
			PREC_LOGICAL_AND = 3,
			PREC_BITWISE_OR = 4,
			PREC_BITWISE_XOR = 5,
			PREC_BITWISE_AND = 6,
			PREC_EQUAL = 7,
			PREC_UNEQUAL = 8,
			PREC_SHIFT = 9,
			PREC_SUM = 10,
			PREC_PRODUCT = 11,
			PREC_POINTER_TO_MEMBER = 12,
			PREC_PREFIX = 13,
			PREC_POSTFIX = 14,
			PREC_SCOPE = 15,
			PREC_PRIMARY = 16
		};

	  public:
		Expression();
		Expression(const Expression&);
		virtual ~Expression();

		void print(std::ostream&) const;

		virtual ExpressionType getExpressionType() const = 0;
		virtual void print(std::ostream&, int) const = 0;
		virtual Expression* cloneExpression() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_EXPRESSION_HPP */
