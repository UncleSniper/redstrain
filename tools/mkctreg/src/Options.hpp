#ifndef REDSTRAIN_TOOLS_MKCTREG_OPTIONS_HPP
#define REDSTRAIN_TOOLS_MKCTREG_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string infile, outfile, ns;

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

	inline const std::string& getNamespace() const {
		return ns;
	}

	void usage();
	void setNamespace(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_MKCTREG_OPTIONS_HPP */
