#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALBASE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALBASE_HPP

#incldue "BuiltinType.hpp"
#include "LiteralExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API IntegerLiteralBase : public LiteralExpression {

	  public:
		IntegerLiteralBase();
		IntegerLiteralBase(const IntegerLiteralBase&);

		virtual LiteralType getLiteralType() const;
		virtual BuiltinType::Primitive getIntegerType() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_INTEGERLITERALBASE_HPP */
