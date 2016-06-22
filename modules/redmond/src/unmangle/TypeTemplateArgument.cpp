#include "Type.hpp"
#include "TypeTemplateArgument.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	TypeTemplateArgument::TypeTemplateArgument(Type* type) : type(type) {}

	TypeTemplateArgument::TypeTemplateArgument(const TypeTemplateArgument& argument)
			: TemplateArgument(argument), type(argument.type->cloneType()) {}

	TypeTemplateArgument::~TypeTemplateArgument() {
		delete type;
	}

	TemplateArgument::ArgumentType TypeTemplateArgument::getArgumentType() const {
		return AT_TYPE;
	}

	void TypeTemplateArgument::print(ostream& out, bool& lastWasGreater,
			const CurrentTemplateArguments& arguments) const {
		type->print(out, lastWasGreater, arguments);
	}

	TemplateArgument* TypeTemplateArgument::cloneTemplateArgument() const {
		return new TypeTemplateArgument(*this);
	}

}}}
