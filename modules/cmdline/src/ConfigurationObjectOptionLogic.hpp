#ifndef REDSTRAIN_MOD_CMDLINE_CONFIGURATIONOBJECTOPTIONLOGIC_HPP
#define REDSTRAIN_MOD_CMDLINE_CONFIGURATIONOBJECTOPTIONLOGIC_HPP

#include <redstrain/util/Delete.hpp>

#include "OptionLogic.hpp"
#include "SwitchMemberFunctionWordAction.hpp"
#include "PresenceMemberFunctionWordAction.hpp"
#include "ArgumentMemberFunctionWordAction.hpp"

namespace redengine {
namespace cmdline {

	template<typename BaseT>
	class ConfigurationObjectOptionLogic : public OptionLogic {

	  private:
		BaseT* base;

	  public:
		ConfigurationObjectOptionLogic(BaseT* base = NULL, int flags = OptionLogic::DEFAULT_FLAGS)
				: OptionLogic(flags), base(base) {}
		ConfigurationObjectOptionLogic(const ConfigurationObjectOptionLogic& logic)
				: OptionLogic(logic), base(logic.base) {}

		inline BaseT* getBaseObject() const {
			return base;
		}

		inline void setBaseObject(BaseT* newBase) {
			base = newBase;
		}

		inline BaseT *const *getBaseObjectHolder() const {
			return &base;
		}

		using OptionLogic::addLongOption;
		using OptionLogic::addShortOption;

		template<typename ReturnT>
		void addShortOption(char symbol, ReturnT (BaseT::*function)(), Arity arity = NO_ARGUMENT) {
			util::Delete<PresenceMemberFunctionWordAction<BaseT, ReturnT> >
					action(new PresenceMemberFunctionWordAction<BaseT, ReturnT>(&base, function));
			addShortOption(symbol, *action, arity, true);
			action.set();
		}

		template<typename ReturnT>
		void addShortOption(char symbol, ReturnT (BaseT::*function)(bool), bool value = true,
				Arity arity = NO_ARGUMENT) {
			util::Delete<SwitchMemberFunctionWordAction<BaseT, ReturnT> >
					action(new SwitchMemberFunctionWordAction<BaseT, ReturnT>(&base, function, value));
			addShortOption(symbol, *action, arity, true);
			action.set();
		}

		template<typename ReturnT>
		void addShortOption(char symbol, ReturnT (BaseT::*function)(const std::string&),
				Arity arity = NO_ARGUMENT) {
			util::Delete<ArgumentMemberFunctionWordAction<BaseT, ReturnT> >
					action(new ArgumentMemberFunctionWordAction<BaseT, ReturnT>(&base, function));
			addShortOption(symbol, *action, arity, true);
			action.set();
		}

		template<typename ReturnT>
		void addLongOption(const std::string& name, ReturnT (BaseT::*function)(),
				Arity arity = NO_ARGUMENT) {
			util::Delete<PresenceMemberFunctionWordAction<BaseT, ReturnT> >
					action(new PresenceMemberFunctionWordAction<BaseT, ReturnT>(&base, function));
			addLongOption(name, *action, arity, true);
			action.set();
		}

		template<typename ReturnT>
		void addLongOption(const std::string& name, ReturnT (BaseT::*function)(bool), bool value = true,
				Arity arity = NO_ARGUMENT) {
			util::Delete<SwitchMemberFunctionWordAction<BaseT, ReturnT> >
					action(new SwitchMemberFunctionWordAction<BaseT, ReturnT>(&base, function, value));
			addLongOption(name, *action, arity, true);
			action.set();
		}

		template<typename ReturnT>
		void addLongOption(const std::string& name, ReturnT (BaseT::*function)(const std::string&),
				Arity arity = NO_ARGUMENT) {
			util::Delete<ArgumentMemberFunctionWordAction<BaseT, ReturnT> >
					action(new ArgumentMemberFunctionWordAction<BaseT, ReturnT>(&base, function));
			addLongOption(name, *action, arity, true);
			action.set();
		}

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_CONFIGURATIONOBJECTOPTIONLOGIC_HPP */
