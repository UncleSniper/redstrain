#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_LOCALNAME_CPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_LOCALNAME_CPP

#include "LocalName.hpp"
#include "CPPSymbol.hpp"
#include "../unmangle-utils.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	LocalName::LocalName(CPPSymbol* function, Name* name, unsigned discriminator)
			: function(function), name(name), discriminator(discriminator) {}

	LocalName::LocalName(const LocalName& name) : Name(name),  discriminator(name.discriminator) {
		UnmanglePtr<CPPSymbol> f(name.function->cloneSymbol());
		UnmanglePtr<Name> n(name.name ? name.name->cloneName() : NULL);
		function = f.ptr;
		this->name = n.ptr;
		f.ptr = NULL;
		n.ptr = NULL;
	}

	LocalName::~LocalName() {
		delete function;
		if(name)
			delete name;
	}

	Name::NameType LocalName::getNameType() const {
		return NT_LOCAL;
	}

	void LocalName::print(ostream& out, bool& lastWasGreater, const CurrentTemplateArguments& arguments,
			const string*) const {
		function->print(out);
		out << "::";
		if(name) {
			lastWasGreater = false;
			name->print(out, lastWasGreater, arguments, NULL);
		}
		else {
			out << "<anonymous>";
			lastWasGreater = true;
		}
		if(discriminator) {
			out << '@' << discriminator;
			lastWasGreater = false;
		}
	}

	Name* LocalName::cloneName() const {
		return new LocalName(*this);
	}

	bool LocalName::namesTemplate() const {
		return name->namesTemplate();
	}

	bool LocalName::namesReturnless() const {
		return name->namesReturnless();
	}

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_LOCALNAME_CPP */
