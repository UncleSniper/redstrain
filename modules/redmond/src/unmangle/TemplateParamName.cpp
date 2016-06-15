#include "TemplateParamName.hpp"

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

	void TemplateParamName::print(ostream& out, bool& lastWasGreater, const string*) const {
		out << '$' << parameter;
		lastWasGreater = false;
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
