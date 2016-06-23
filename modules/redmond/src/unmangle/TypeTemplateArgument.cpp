#include "Type.hpp"
#include "TypeTemplateArgument.hpp"

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

	void TypeTemplateArgument::print(SymbolSink& sink, const CurrentTemplateArguments& arguments) const {
		type->print(sink, arguments);
	}

	TemplateArgument* TypeTemplateArgument::cloneTemplateArgument() const {
		return new TypeTemplateArgument(*this);
	}

}}}
