#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_SIZEOFTYPEEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_SIZEOFTYPEEXPRESSION_HPP

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Type;

	class REDSTRAIN_REDMOND_API SizeOfTypeExpression : public Expression {

	  private:
		Type* type;

	  protected:
		virtual void print(std::ostream&, int) const;

	  public:
		SizeOfTypeExpression(Type*);
		SizeOfTypeExpression(const SizeOfTypeExpression&);
		virtual ~SizeOfTypeExpression();

		inline Type& getType() const {
			return *type;
		}

		virtual ExpressionType getExpressionType() const;
		virtual Expression* cloneExpression() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_SIZEOFTYPEEXPRESSION_HPP */
