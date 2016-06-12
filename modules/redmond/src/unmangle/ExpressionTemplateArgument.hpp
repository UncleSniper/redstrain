#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_EXPRESSIONTEMPLATEARGUMENT_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_EXPRESSIONTEMPLATEARGUMENT_HPP

#include "TemplateArgument.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Expression;

	class REDSTRAIN_REDMOND_API ExpressionTemplateArgument : public TemplateArgument {

	  private:
		Expression* expression;

	  public:
		ExpressionTemplateArgument(Expression*);
		ExpressionTemplateArgument(const ExpressionTemplateArgument&);
		virtual ~ExpressionTemplateArgument();

		inline Expression& getExpression() const {
			return *expression;
		}

		virtual ArgumentType getArgumentType() const;
		virtual void print(std::ostream&, bool&) const;
		virtual TemplateArgument* cloneTemplateArgument() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_EXPRESSIONTEMPLATEARGUMENT_HPP */
