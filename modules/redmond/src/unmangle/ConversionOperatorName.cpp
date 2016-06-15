#include "Type.hpp"
#include "ConversionOperatorName.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	ConversionOperatorName::ConversionOperatorName(Type* targetType) : targetType(targetType) {}

	ConversionOperatorName::ConversionOperatorName(const ConversionOperatorName& name)
			: UnqualifiedName(name), targetType(name.targetType->cloneType()) {}

	ConversionOperatorName::~ConversionOperatorName() {
		delete targetType;
	}

	Name::NameType ConversionOperatorName::getNameType() const {
		return NT_CONVERSION_OPERATOR;
	}

	void ConversionOperatorName::print(ostream& out, bool& lastWasGreater, const string*) const {
		out << "operator ";
		lastWasGreater = false;
		targetType->print(out, lastWasGreater);
	}

	Name* ConversionOperatorName::cloneName() const {
		return new ConversionOperatorName(*this);
	}

	bool ConversionOperatorName::namesTemplate() const {
		return false;
	}

	bool ConversionOperatorName::namesReturnless() const {
		return true;
	}

}}}
