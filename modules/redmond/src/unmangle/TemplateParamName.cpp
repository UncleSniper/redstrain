#include "Type.hpp"
#include "SymbolSink.hpp"
#include "TemplateParamName.hpp"
#include "TypeTemplateArgument.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateParamName::TemplateParamName(unsigned parameter) : parameter(parameter) {}

	TemplateParamName::TemplateParamName(const TemplateParamName& name)
			: UnqualifiedName(name), parameter(name.parameter) {}

	Name::NameType TemplateParamName::getNameType() const {
		return NT_TEMPLATE_PARAM;
	}

	void TemplateParamName::print(SymbolSink& sink, const CurrentTemplateArguments& arguments, const string*) const {
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

	Name* TemplateParamName::cloneName() const {
		return new TemplateParamName(*this);
	}

	bool TemplateParamName::namesTemplate() const {
		return false;
	}

	bool TemplateParamName::namesReturnless() const {
		return false;
	}

}}}
