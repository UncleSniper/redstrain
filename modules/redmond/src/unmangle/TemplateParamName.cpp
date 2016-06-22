#include "Type.hpp"
#include "TemplateParamName.hpp"
#include "TypeTemplateArgument.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateParamName::TemplateParamName(unsigned parameter) : parameter(parameter) {}

	TemplateParamName::TemplateParamName(const TemplateParamName& name)
			: UnqualifiedName(name), parameter(name.parameter) {}

	Name::NameType TemplateParamName::getNameType() const {
		return NT_TEMPLATE_PARAM;
	}

	void TemplateParamName::print(ostream& out, bool& lastWasGreater, const CurrentTemplateArguments& arguments,
			const string*) const {
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
