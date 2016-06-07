#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_FUNCTIONTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_FUNCTIONTYPE_HPP

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class BareFunctionType;

	class REDSTRAIN_REDMOND_API FunctionType : public Type {

	  private:
		bool externC;
		BareFunctionType* type;

	  public:
		FunctionType(bool, BareFunctionType*);
		FunctionType(const FunctionType&);
		virtual ~FunctionType();

		inline bool isExternC() const {
			return externC;
		}

		inline BareFunctionType& getFunctionType() const {
			return *type;
		}

		virtual TypeType getTypeType() const;
		virtual void print(std::ostream&, bool&) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_FUNCTIONTYPE_HPP */
