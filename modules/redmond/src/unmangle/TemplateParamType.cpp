#include "TemplateParamType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateParamType::TemplateParamType(unsigned parameter) : parameter(parameter) {}

	TemplateParamType::TemplateParamType(const TemplateParamType& type) : Type(type), parameter(type.parameter) {}

	Type::TypeType TemplateParamType::getTypeType() const {
		return TT_TEMPLATE_PARAM;
	}

	void TemplateParamType::print(ostream& out, bool& lastWasGreater) const {
		out << '$' << parameter;
		lastWasGreater = false;
	}

	Type* TemplateParamType::cloneType() const {
		return new TemplateParamType(*this);
	}

}}}
