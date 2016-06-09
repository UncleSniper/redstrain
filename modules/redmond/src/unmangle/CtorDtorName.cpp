#include "CtorDtorName.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	CtorDtorName::CtorDtorName(Function function) : function(function) {}

	CtorDtorName::CtorDtorName(const CtorDtorName& name) : UnqualifiedName(name), function(name.function) {}

	Name::NameType CtorDtorName::getNameType() const {
		return NT_CTOR_DTOR;
	}

	void CtorDtorName::print(ostream& out, bool& lastWasGreater, const string* className) const {
		switch(function) {
			case FN_COMPLETE_CTOR:
			case FN_BASE_CTOR:
			case FN_ALLOCATING_CTOR:
				if(className && !className->empty()) {
					out << *className;
					lastWasGreater = false;
				}
				else {
					out << "<constructor>";
					lastWasGreater = true;
				}
				break;
			case FN_DELETING_DTOR:
			case FN_COMPLETE_DTOR:
			case FN_BASE_DTOR:
				if(className && !className->empty()) {
					out << '~' << << *className;
					lastWasGreater = false;
				}
				else {
					out << "<destructor>";
					lastWasGreater = true;
				}
				break;
			default:
				out << "<unknown constructor-like>";
				lastWasGreater = true;
				break;
		}
	}

	Name* CtorDtorName::cloneName() const {
		return new CtorDtorName(*this);
	}

}}}
