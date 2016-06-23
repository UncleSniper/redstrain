#include "SymbolSink.hpp"
#include "TypeTemplateArgument.hpp"
#include "TemplateTemplateParamType.hpp"
#include "../unmangle-utils.hpp"

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

	void TemplateTemplateParamType::print(SymbolSink& sink, const CurrentTemplateArguments& targuments,
			const Type*) const {
		if(parameter >= static_cast<unsigned>(targuments.size()))
			sink.putUndefinedTemplateParameter(parameter);
		else {
			const TemplateArgument& a = *targuments[parameter];
			if(a.getArgumentType() == TemplateArgument::AT_TYPE) {
				CurrentTemplateArguments empty;
				static_cast<const TypeTemplateArgument&>(a).getType().print(sink, empty);
			}
			else
				sink.putUndefinedTemplateParameter(parameter);
		}
		sink.putSeparator(SymbolSink::SEP_LEFT_ANGLE);
		ArgumentIterator abegin(arguments.begin()), aend(arguments.end());
		unsigned space = sink.getRemainingColumnCount();
		bool breakArgs = false, first = true;
		if(space) {
			unsigned iwidth = 0u;
			ArgumentIterator sabegin(abegin), saend(aend);
			for(; sabegin != saend; ++sabegin) {
				if(first)
					first = false;
				else
					iwidth += sink.getInlineWidthOf(SymbolSink::SEP_COMMA)
							+ sink.getInlineWidthOf(SymbolSink::SEP_AFTER_COMMA);
				iwidth += sink.getInlineWidthOf(**sabegin, targuments);
			}
			first = false;
			breakArgs = iwidth > space;
		}
		for(; abegin != aend; ++abegin) {
			if(first) {
				first = false;
				if(breakArgs)
					sink.startNewLine(1);
			}
			else {
				sink.putSeparator(SymbolSink::SEP_COMMA);
				if(breakArgs)
					sink.startNewLine(0);
				else
					sink.putSeparator(SymbolSink::SEP_AFTER_COMMA);
			}
			(*abegin)->print(sink, targuments);
		}
		if(breakArgs && !first)
			sink.startNewLine(-1);
		sink.putSeparator(SymbolSink::SEP_RIGHT_ANGLE);
	}

	Type* TemplateTemplateParamType::cloneType() const {
		return new TemplateTemplateParamType(*this);
	}

}}}
