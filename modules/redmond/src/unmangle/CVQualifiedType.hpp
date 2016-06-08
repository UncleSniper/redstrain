#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_CVQUALIFIEDTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_CVQUALIFIEDTYPE_HPP

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API CVQualifiedType : public Type {

	  public:
		static const int CVQ_RESTRICT = 01;
		static const int CVQ_VOLATILE = 02;
		static const int CVQ_CONST    = 04;

	  private:
		int qualifiers;
		Type* type;

	  public:
		CVQualifiedType(int, Type*);
		CVQualifiedType(const CVQualifiedType&);
		virtual ~CVQualifiedType();

		virtual TypeType getTypeType() const;
		virtual void print(std::ostream&, bool&) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_CVQUALIFIEDTYPE_HPP */
