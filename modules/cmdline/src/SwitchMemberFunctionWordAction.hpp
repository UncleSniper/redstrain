#ifndef REDSTRAIN_MOD_CMDLINE_SWITCHMEMBERFUNCTIONWORDACTION_HPP
#define REDSTRAIN_MOD_CMDLINE_SWITCHMEMBERFUNCTIONWORDACTION_HPP

#include "MemberFunctionWordAction.hpp"

namespace redengine {
namespace cmdline {

	template<typename BaseT, typename ReturnT = void>
	class SwitchMemberFunctionWordAction : public MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)(bool)> {

	  private:
		bool value;

	  public:
		SwitchMemberFunctionWordAction(BaseT *const *base, ReturnT (BaseT::*function)(bool),
				bool value = true)
				: MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)(bool)>(base, function),
				value(value) {}
		SwitchMemberFunctionWordAction(const SwitchMemberFunctionWordAction& action)
				: MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)(bool)>(action), value(action.value) {}

		inline bool getValue() const {
			return value;
		}

		inline void setValue(bool newValue) {
			value = newValue;
		}

		virtual void wordEncountered(const std::string&, const std::string&) {
			((*this->base)->*this->function)(value);
		}

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_SWITCHMEMBERFUNCTIONWORDACTION_HPP */
