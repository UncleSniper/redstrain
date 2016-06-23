#include "SymbolSink.hpp"
#include "OperatorName.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	OperatorName::OperatorName(Operator oper) : oper(oper) {}

	OperatorName::OperatorName(const OperatorName& name) : UnqualifiedName(name), oper(name.oper) {}

	Name::NameType OperatorName::getNameType() const {
		return NT_OPERATOR;
	}

	void OperatorName::print(SymbolSink& sink, const CurrentTemplateArguments&, const string*) const {
		sink.putOperatorName(*this);
	}

	Name* OperatorName::cloneName() const {
		return new OperatorName(*this);
	}

	bool OperatorName::namesTemplate() const {
		return false;
	}

	bool OperatorName::namesReturnless() const {
		return false;
	}

}}}
