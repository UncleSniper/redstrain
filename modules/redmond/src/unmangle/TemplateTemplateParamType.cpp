#include "TemplateArgument.hpp"
#include "TemplateTemplateParamType.hpp"
#include "../unmangle-utils.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	TemplateTemplateParamType::TemplateTemplateParamType(unsigned parameter) : parameter(parameter) {}

	TemplateTemplateParamType::TemplateTemplateParamType(const TemplateTemplateParamType& type)
			: Type(type), parameter(type.parameter) {
		UnmangleCollectionPtr<Arguments> delArgs(&arguments);
		ArgumentIterator begin(type.arguments.begin()), end(type.arguments.end());
		UnmanglePtr<TemplateArgument> arg(NULL);
		for(; begin != end; ++begin) {
			arg.ptr = (*begin)->cloneTemplateArgument();
			arguments.push_back(arg.ptr);
			arg.ptr = NULL;
		}
		delArgs.ptr = NULL;
	}

	TemplateTemplateParamType::~TemplateTemplateParamType() {
		ArgumentIterator begin(arguments.begin()), end(arguments.end());
		for(; begin != end; ++begin)
			delete *begin;
	}

	unsigned TemplateTemplateParamType::getArgumentCount() const {
		return static_cast<unsigned>(arguments.size());
	}

	void TemplateTemplateParamType::getArguments(ArgumentIterator& begin, ArgumentIterator& end) const {
		begin = arguments.begin();
		end = arguments.end();
	}

	void TemplateTemplateParamType::addArgument(TemplateArgument& argument) {
		arguments.push_back(&argument);
	}

	Type::TypeType TemplateTemplateParamType::getTypeType() const {
		return TT_TEMPLATE_TEMPLATE_PARAM;
	}

	void TemplateTemplateParamType::print(ostream& out, bool& lastWasGreater) const {
		out << '$' << parameter << '<';
		lastWasGreater = false;
		ArgumentIterator abegin(arguments.begin()), aend(arguments.end());
		bool first = true;
		for(; abegin != aend; ++abegin) {
			if(first)
				first = false;
			else
				out << ", ";
			lastWasGreater = false;
			(*abegin)->print(out, lastWasGreater);
		}
		if(lastWasGreater)
			out << ' ';
		else
			lastWasGreater = true;
		out << '>';
	}

	Type* TemplateTemplateParamType::cloneType() const {
		return new TemplateTemplateParamType(*this);
	}

}}}
