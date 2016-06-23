#include "SymbolSink.hpp"
#include "TemplateParamType.hpp"
#include "TypeTemplateArgument.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateParamType::TemplateParamType(unsigned parameter) : parameter(parameter) {}

	TemplateParamType::TemplateParamType(const TemplateParamType& type) : Type(type), parameter(type.parameter) {}

	Type::TypeType TemplateParamType::getTypeType() const {
		return TT_TEMPLATE_PARAM;
	}

	void TemplateParamType::print(SymbolSink& sink, const CurrentTemplateArguments& arguments, const Type*) const {
		if(parameter >= static_cast<unsigned>(arguments.size()))
			sink.putUndefinedTemplateParameter(parameter);
		else {
			const TemplateArgument& a = *arguments[parameter];
			if(a.getArgumentType() == TemplateArgument::AT_TYPE) {
				CurrentTemplateArguments empty;
				static_cast<const TypeTemplateArgument&>(a).getType().print(sink, empty);
			}
			else
				sink.putUndefinedTemplateParameter(parameter);
		}
	}

	Type* TemplateParamType::cloneType() const {
		return new TemplateParamType(*this);
	}

}}}
