#include "Type.hpp"
#include "SymbolSink.hpp"
#include "ConversionOperatorName.hpp"

using std::string;

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

	void ConversionOperatorName::print(SymbolSink& sink, const CurrentTemplateArguments& arguments,
			const string*) const {
		sink.putReserved(SymbolSink::RSV_CONVERSION_OPERATOR);
		targetType->print(sink, arguments);
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
