#ifndef REDSTRAIN_TOOLS_CUNT_OPTIONS_HPP
#define REDSTRAIN_TOOLS_CUNT_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string infile, outfile;

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

	void usage();
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_CUNT_OPTIONS_HPP */
