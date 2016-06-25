#ifndef REDSTRAIN_TOOLS_UNMANGLE_UNMANGLER_HPP
#define REDSTRAIN_TOOLS_UNMANGLE_UNMANGLER_HPP

#include <redstrain/redmond/unmangle/CPPSymbol.hpp>

class Unmangler {

  public:
	typedef redengine::redmond::unmangle::CPPSymbol* (*UnmangleFunction)(const std::string&);
	typedef bool (*SymbolCharPredicate)(char);

  private:
	UnmangleFunction unmangleCB;
	std::string symInitiator;
	SymbolCharPredicate symCharP;

  public:
	Unmangler();
	Unmangler(UnmangleFunction, const std::string&, SymbolCharPredicate);
	Unmangler(const Unmangler&);

	Unmangler& operator=(const Unmangler&);

	redengine::redmond::unmangle::CPPSymbol* unmanglePlainSymbol(const std::string&) const;
	redengine::redmond::unmangle::CPPSymbol* filterUnmangling(const std::string&, std::string::size_type,
			std::string::size_type&, std::string::size_type&) const;

};

#endif /* REDSTRAIN_TOOLS_UNMANGLE_UNMANGLER_HPP */
