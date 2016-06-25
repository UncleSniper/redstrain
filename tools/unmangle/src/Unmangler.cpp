#include "Unmangler.hpp"

using std::string;
using redengine::redmond::unmangle::CPPSymbol;

Unmangler::Unmangler() : unmangleCB(NULL), symCharP(NULL) {}

Unmangler::Unmangler(UnmangleFunction unmangleCB, const string& symInitiator, SymbolCharPredicate symCharP)
		: unmangleCB(unmangleCB), symInitiator(symInitiator), symCharP(symCharP) {}

Unmangler::Unmangler(const Unmangler& unmangler)
		: unmangleCB(unmangler.unmangleCB), symInitiator(unmangler.symInitiator), symCharP(unmangler.symCharP) {}

Unmangler& Unmangler::operator=(const Unmangler& other) {
	unmangleCB = other.unmangleCB;
	symInitiator = other.symInitiator;
	symCharP = other.symCharP;
	return *this;
}

CPPSymbol* Unmangler::unmanglePlainSymbol(const string& mangled) const {
	if(!unmangleCB)
		return NULL;
	return unmangleCB(mangled);
}

CPPSymbol* Unmangler::filterUnmangling(const string& line, string::size_type searchStart,
		string::size_type& foundStart, string::size_type& foundLength) const {
	if(symInitiator.empty() || !unmangleCB || !symCharP)
		return NULL;
	foundStart = line.find(symInitiator, searchStart);
	if(foundStart == string::npos)
		return NULL;
	string::size_type rbound = foundStart + symInitiator.length();
	string::size_type llen = line.length();
	while(rbound < llen && symCharP(line[rbound]))
		++rbound;
	foundLength = rbound - foundStart;
	return unmangleCB(line.substr(foundStart, foundLength));
}
