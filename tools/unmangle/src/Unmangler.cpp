#include <redstrain/util/Delete.hpp>

#include "Unmangler.hpp"

using std::string;
using redengine::util::Delete;
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

string Unmangler::unmanglePlainSymbol(const string& mangled) const {
	if(!unmangleCB)
		return mangled;
	Delete<CPPSymbol> tree(unmangleCB(mangled));
	if(!*tree)
		return mangled;
	return tree->toString();
}

bool Unmangler::filterUnmangling(const string& line, string::size_type searchStart,
		string::size_type& foundStart, string::size_type& foundLength, string& unmangled) const {
	if(symInitiator.empty() || !unmangleCB || !symCharP)
		return false;
	foundStart = line.find(symInitiator, searchStart);
	if(foundStart == string::npos)
		return false;
	string::size_type rbound = foundStart + symInitiator.length();
	string::size_type llen = line.length();
	while(rbound < llen && symCharP(line[rbound]))
		++rbound;
	foundLength = rbound - foundStart;
	Delete<CPPSymbol> tree(unmangleCB(line.substr(foundStart, foundLength)));
	if(!*tree)
		return false;
	unmangled = tree->toString();
	return true;
}
