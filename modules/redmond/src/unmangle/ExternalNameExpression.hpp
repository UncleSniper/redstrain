#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_EXTERNALNAMEEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_EXTERNALNAMEEXPRESSION_HPP

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class CPPSymbol;

	class REDSTRAIN_REDMOND_API ExternalNameExpression : public Expression {

	  private:
		CPPSymbol* symbol;

	  public:
		ExternalNameExpression(CPPSymbol*);
		ExternalNameExpression(const ExternalNameExpression&);
		virtual ~ExternalNameExpression();

		inline CPPSymbol& getSymbol() const {
			return *symbol;
		}

		virtual ExpressionType getExpressionType() const;
		virtual void print(std::ostream&, int) const;
		virtual Expression* cloneExpression() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_EXTERNALNAMEEXPRESSION_HPP */
