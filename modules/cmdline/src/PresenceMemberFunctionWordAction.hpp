#ifndef REDSTRAIN_MOD_CMDLINE_PRESENCEMEMBERFUNCTIONWORDACTION_HPP
#define REDSTRAIN_MOD_CMDLINE_PRESENCEMEMBERFUNCTIONWORDACTION_HPP

#include "MemberFunctionWordAction.hpp"

namespace redengine {
namespace cmdline {

	template<typename BaseT, typename ReturnT = void>
	class PresenceMemberFunctionWordAction : public MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)()> {

	  public:
		PresenceMemberFunctionWordAction(BaseT *const *base, ReturnT (BaseT::*function)())
				: MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)()>(base, function) {}
		PresenceMemberFunctionWordAction(const PresenceMemberFunctionWordAction& action)
				: MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)()>(action) {}

		virtual void wordEncountered(const std::string&, const std::string&) {
			((*this->base)->*this->function)();
		}

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_PRESENCEMEMBERFUNCTIONWORDACTION_HPP */
