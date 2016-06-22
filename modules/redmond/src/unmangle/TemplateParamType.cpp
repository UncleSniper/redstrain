#include "TemplateParamType.hpp"
#include "TypeTemplateArgument.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateParamType::TemplateParamType(unsigned parameter) : parameter(parameter) {}

	TemplateParamType::TemplateParamType(const TemplateParamType& type) : Type(type), parameter(type.parameter) {}

	Type::TypeType TemplateParamType::getTypeType() const {
		return TT_TEMPLATE_PARAM;
	}

	void TemplateParamType::print(ostream& out, bool& lastWasGreater, const CurrentTemplateArguments& arguments,
			const Type*) const {
		if(parameter >= static_cast<unsigned>(arguments.size())) {
			out << '$' << parameter;
			lastWasGreater = false;
		}
		else {
			const TemplateArgument& a = *arguments[parameter];
			if(a.getArgumentType() == TemplateArgument::AT_TYPE) {
				CurrentTemplateArguments empty;
				static_cast<const TypeTemplateArgument&>(a).getType().print(out, lastWasGreater, empty);
			}
			else {
				out << '$' << parameter;
				lastWasGreater = false;
			}
		}
	}

	Type* TemplateParamType::cloneType() const {
		return new TemplateParamType(*this);
	}

}}}
