#ifndef REDSTRAIN_TOOLS_UNMANGLE_OPTIONS_HPP
#define REDSTRAIN_TOOLS_UNMANGLE_OPTIONS_HPP

#include <string>

class Options {

  private:
	const std::string progname;
	unsigned barewordCount;
	std::string scheme, symbol;
	bool nowrap;

  private:
	Options(const Options&);

  public:
	Options(const char*);

	inline const std::string& getScheme() const {
		return scheme;
	}

	inline bool hasSymbol() const {
		return barewordCount >= 2u;
	}

	inline const std::string& getSymbol() const {
		return symbol;
	}

	inline bool shouldAvoidWrapping() const {
		return nowrap;
	}

	void usage();
	void addBareword(const std::string&);
	void setAvoidWrapping(bool);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_UNMANGLE_OPTIONS_HPP */
