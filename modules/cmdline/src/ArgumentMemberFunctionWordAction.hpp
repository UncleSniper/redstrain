#ifndef REDSTRAIN_MOD_CMDLINE_ARGUMENTMEMBERFUNCTIONWORDACTION_HPP
#define REDSTRAIN_MOD_CMDLINE_ARGUMENTMEMBERFUNCTIONWORDACTION_HPP

#include <redstrain/util/MemberFunctionPointer.hpp>

#include "MemberFunctionWordAction.hpp"

namespace redengine {
namespace cmdline {

	template<typename BaseT, typename ReturnT = void>
	class ArgumentMemberFunctionWordAction
			: public MemberFunctionWordAction<
				BaseT,
				typename util::MemberFunctionPointer<BaseT, ReturnT, const std::string&>::Type
			> {

	  private:
		typedef util::MemberFunctionPointer<BaseT, ReturnT, const std::string&> MFP;

	  public:
		typedef typename MFP::Type Callback;

	  public:
		ArgumentMemberFunctionWordAction(BaseT *const *base, Callback function)
				: MemberFunctionWordAction<BaseT, Callback>(base, function) {}
		ArgumentMemberFunctionWordAction(const ArgumentMemberFunctionWordAction& action)
				: MemberFunctionWordAction<BaseT, Callback>(action) {}

		virtual void wordEncountered(const std::string&, const std::string& value) {
			MFP::call(this->function, **this->base, value);
		}

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_ARGUMENTMEMBERFUNCTIONWORDACTION_HPP */
