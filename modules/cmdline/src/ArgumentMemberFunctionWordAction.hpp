#ifndef REDSTRAIN_MOD_CMDLINE_ARGUMENTMEMBERFUNCTIONWORDACTION_HPP
#define REDSTRAIN_MOD_CMDLINE_ARGUMENTMEMBERFUNCTIONWORDACTION_HPP

#include "MemberFunctionWordAction.hpp"

namespace redengine {
namespace cmdline {

	template<typename BaseT, typename ReturnT = void>
	class ArgumentMemberFunctionWordAction
			: public MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)(const std::string&)> {

	  public:
		ArgumentMemberFunctionWordAction(BaseT *const *base,
				ReturnT (BaseT::*function)(const std::string&))
				: MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)(const std::string&)>(base, function) {}
		ArgumentMemberFunctionWordAction(const ArgumentMemberFunctionWordAction& action)
				: MemberFunctionWordAction<BaseT, ReturnT (BaseT::*)(const std::string&)>(action) {}

		virtual void wordEncountered(const std::string&, const std::string& value) {
			((*this->base)->*this->function)(value);
		}

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_ARGUMENTMEMBERFUNCTIONWORDACTION_HPP */
