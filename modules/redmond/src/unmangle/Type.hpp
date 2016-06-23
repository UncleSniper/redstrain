#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TYPE_HPP

#include <iostream>

#include "types.hpp"
#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class SymbolSink;

	class REDSTRAIN_REDMOND_API Type {

	  public:
		enum TypeType {
			TT_BUILTIN,
			TT_VENDOR_EXTENDED,
			TT_FUNCTION,
			TT_ARRAY,
			TT_POINTER_TO_MEMBER,
			TT_CV_QUALIFIED,
			TT_MODIFIED,
			TT_ENUM,
			TT_TEMPLATE_PARAM,
			TT_TEMPLATE_TEMPLATE_PARAM,
			TT_VENDOR_EXTENDED_QUALIFIED
		};

	  public:
		Type();
		Type(const Type&);
		virtual ~Type();

		void print(SymbolSink&, const CurrentTemplateArguments&) const;

		virtual TypeType getTypeType() const = 0;
		virtual bool inlinesEnclosingClassName() const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&, const Type*) const = 0;
		virtual Type* cloneType() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TYPE_HPP */
