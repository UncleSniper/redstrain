#include "SymbolSink.hpp"
#include "CtorDtorName.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	CtorDtorName::CtorDtorName(Function function) : function(function) {}

	CtorDtorName::CtorDtorName(const CtorDtorName& name) : UnqualifiedName(name), function(name.function) {}

	Name::NameType CtorDtorName::getNameType() const {
		return NT_CTOR_DTOR;
	}

	void CtorDtorName::print(SymbolSink& sink, const CurrentTemplateArguments&, const string* className) const {
		switch(function) {
			case FN_COMPLETE_CTOR:
			case FN_BASE_CTOR:
			case FN_ALLOCATING_CTOR:
				if(className && !className->empty())
					sink.putSourceName(*className);
				else
					sink.putReplacementString(SymbolSink::RS_CONSTRUCTOR);
				break;
			case FN_DELETING_DTOR:
			case FN_COMPLETE_DTOR:
			case FN_BASE_DTOR:
				if(className && !className->empty()) {
					sink.putSeparator(SymbolSink::SEP_DESTRUCTOR_TILDE);
					sink.putSourceName(*className);
				}
				else
					sink.putReplacementString(SymbolSink::RS_DESTRUCTOR);
				break;
			default:
				sink.putReplacementString(SymbolSink::RS_UNKNOWN_CONSTRUCTOR_LIKE);
				break;
		}
	}

	Name* CtorDtorName::cloneName() const {
		return new CtorDtorName(*this);
	}

	bool CtorDtorName::namesTemplate() const {
		return false;
	}

	bool CtorDtorName::namesReturnless() const {
		return true;
	}

}}}
