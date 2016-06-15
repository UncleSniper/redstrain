#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMLITERALEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMLITERALEXPRESSION_HPP

#include "LiteralExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Name;

	class REDSTRAIN_REDMOND_API EnumLiteralExpression : public LiteralExpression {

	  private:
		Name* typeName;
		int value;

	  public:
		EnumLiteralExpression(Name*, int);
		EnumLiteralExpression(const EnumLiteralExpression&);
		virtual ~EnumLiteralExpression();

		virtual LiteralType getLiteralType() const;
		virtual void print(std::ostream&, int) const;
		virtual Expression* cloneExpression() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMLITERALEXPRESSION_HPP */
