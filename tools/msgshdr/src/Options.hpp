#ifndef REDSTRAIN_TOOLS_MSGSHDR_OPTIONS_HPP
#define REDSTRAIN_TOOLS_MSGSHDR_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string infile, outfile, inenc, type, constprefix, guard;

  private:
	Options(const Options&);

  public:
	Options(const char*);

	inline const std::string& getInputFile() const {
		return infile;
	}

	inline const std::string& getOutputFile() const {
		return outfile;
	}

	inline const std::string& getInputFileEncoding() const {
		return inenc;
	}

	inline const std::string& getTypeName() const {
		return type;
	}

	inline const std::string& getConstantPrefix() const {
		return constprefix;
	}

	inline const std::string& getGuardMacro() const {
		return guard;
	}

	void usage();
	void setInputFileEncoding(const std::string&);
	void setTypeName(const std::string&);
	void setConstantPrefix(const std::string&);
	void setGuardMacro(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_MSGSHDR_OPTIONS_HPP */
