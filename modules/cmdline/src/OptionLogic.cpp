#include "WordAction.hpp"
#include "OptionLogic.hpp"

using std::string;

namespace redengine {
namespace cmdline {

	OptionLogic::Option::Option() : arity(NO_ARGUMENT), action(NULL), managed(false) {}

	OptionLogic::Option::Option(Arity arity, WordAction* action, bool managed)
			: arity(arity), action(action), managed(managed) {}

	OptionLogic::Option::Option(const Option& option)
			: arity(option.arity), action(option.action), managed(option.managed) {}

	OptionLogic::Option::~Option() {
		if(managed && action)
			delete action;
	}

	OptionLogic::Option& OptionLogic::Option::operator=(const Option& option) {
		if(managed && action)
			delete action;
		arity = option.arity;
		action = option.action;
		managed = option.managed;
		return *this;
	}

	void OptionLogic::Option::assign(Arity newArity, WordAction* newAction, bool newManaged) {
		if(managed && action)
			delete action;
		arity = newArity;
		action = newAction;
		managed = newManaged;
	}

	OptionLogic::OptionLogic(int flags)
			: flags(flags), initiatorSymbol('-'), separatorSymbol('='), optionTerminator("--"),
			inlineLongConflict(INLINE_LONG_CONFLICT_IS_ERROR) {}

	OptionLogic::OptionLogic(const OptionLogic& logic)
			: flags(logic.flags), initiatorSymbol(logic.initiatorSymbol),
			separatorSymbol(logic.separatorSymbol), optionTerminator(logic.optionTerminator),
			inlineLongConflict(logic.inlineLongConflict) {
		ShortOptionIterator sbegin(logic.shortOptions.begin()), send(logic.shortOptions.end());
		for(; sbegin != send; ++sbegin)
			shortOptions[sbegin->first]
					= Option(sbegin->second.getArity(), sbegin->second.getAction(), false);
		LongOptionIterator lbegin(logic.longOptions.begin()), lend(logic.longOptions.end());
		for(; lbegin != lend; ++lbegin)
			longOptions[lbegin->first]
					= Option(lbegin->second.getArity(), lbegin->second.getAction(), false);
	}

	void OptionLogic::enableFlags(Flags flag) {
		flags |= flag;
	}

	void OptionLogic::disableFlags(Flags flag) {
		flags &= ~static_cast<int>(flag);
	}

	bool OptionLogic::hasFlags(Flags flag) const {
		return (flags & flag) == flag;
	}

	void OptionLogic::addShortOption(char symbol, WordAction* action, Arity arity, bool managed) {
		shortOptions[symbol].assign(arity, action, managed);
	}

	void OptionLogic::addLongOption(const string& name, WordAction* action, Arity arity, bool managed) {
		longOptions[name].assign(arity, action, managed);
	}

	const OptionLogic::Option* OptionLogic::getShortOption(char symbol) const {
		ShortOptionIterator it = shortOptions.find(symbol);
		return it == shortOptions.end() ? NULL : &it->second;
	}

	const OptionLogic::Option* OptionLogic::getLongOption(const string& name) const {
		LongOptionIterator it = longOptions.find(name);
		return it == longOptions.end() ? NULL : &it->second;
	}

}}
