#include "SymbolSink.hpp"
#include "CovariantOverrideThunkSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	CovariantOverrideThunkSymbol::CovariantOverrideThunkSymbol(const CallOffset& thisAdjustment,
			const CallOffset& resultAdjustment, CPPSymbol* targetFunction)
			: thisAdjustment(thisAdjustment), resultAdjustment(resultAdjustment), targetFunction(targetFunction) {}

	CovariantOverrideThunkSymbol::CovariantOverrideThunkSymbol(const CovariantOverrideThunkSymbol& symbol)
			: SpecialSymbol(symbol), thisAdjustment(symbol.thisAdjustment),
			resultAdjustment(symbol.resultAdjustment), targetFunction(symbol.targetFunction->cloneSymbol()) {}

	CovariantOverrideThunkSymbol::~CovariantOverrideThunkSymbol() {
		delete targetFunction;
	}

	SpecialSymbol::SpecialSymbolType CovariantOverrideThunkSymbol::getSpecialSymbolType() const {
		return SST_COVARIANT_OVERRIDE_THUNK;
	}

	CPPSymbol* CovariantOverrideThunkSymbol::cloneSymbol() const {
		return new CovariantOverrideThunkSymbol(*this);
	}

	void CovariantOverrideThunkSymbol::print(SymbolSink& sink) const {
		sink.putCovariantOverrideThunk(*this);
	}

}}}
