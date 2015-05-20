#ifndef REDSTRAIN_MOD_CMDLINE_MEMBERFUNCTIONWORDACTION_HPP
#define REDSTRAIN_MOD_CMDLINE_MEMBERFUNCTIONWORDACTION_HPP

#include "WordAction.hpp"

namespace redengine {
namespace cmdline {

	template<typename BaseT, typename FunctionT>
	class MemberFunctionWordAction : public WordAction {

	  public:
		typedef BaseT Base;
		typedef FunctionT Function;

	  protected:
		BaseT *const *base;
		FunctionT function;

	  public:
		MemberFunctionWordAction(BaseT *const *base, FunctionT function)
				: base(base), function(function) {}
		MemberFunctionWordAction(const MemberFunctionWordAction& action)
				: base(action.base), function(action.function) {}

		inline BaseT *const *getBaseObject() const {
			return base;
		}

		inline void setBaseObject(BaseT *const *newBase) {
			base = newBase;
		}

		inline FunctionT getFunction() const {
			return function;
		}

		inline void setFunction(FunctionT newFunction) {
			newFunction = function;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_MEMBERFUNCTIONWORDACTION_HPP */
