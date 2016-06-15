#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_CTORDTORNAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_CTORDTORNAME_HPP

#include "UnqualifiedName.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API CtorDtorName : public UnqualifiedName {

	  public:
		enum Function {
			FN_COMPLETE_CTOR,
			FN_BASE_CTOR,
			FN_ALLOCATING_CTOR,
			FN_DELETING_DTOR,
			FN_COMPLETE_DTOR,
			FN_BASE_DTOR
		};

	  private:
		Function function;

	  public:
		CtorDtorName(Function);
		CtorDtorName(const CtorDtorName&);

		virtual NameType getNameType() const;
		virtual void print(std::ostream&, bool&, const std::string*) const;
		virtual Name* cloneName() const;
		virtual bool namesTemplate() const;
		virtual bool namesReturnless() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_CTORDTORNAME_HPP */
