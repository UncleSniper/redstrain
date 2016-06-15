#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALBASE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALBASE_HPP

#include "BuiltinType.hpp"
#include "LiteralExpression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API FloatLiteralBase : public LiteralExpression {

	  public:
		FloatLiteralBase();
		FloatLiteralBase(const FloatLiteralBase&);

		virtual LiteralType getLiteralType() const;
		virtual BuiltinType::Primitive getFloatType() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_FLOATLITERALBASE_HPP */
