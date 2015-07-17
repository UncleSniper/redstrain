#ifndef REDSTRAIN_TOOLS_LNBLOB_OPTIONS_HPP
#define REDSTRAIN_TOOLS_LNBLOB_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string infile, outfile, pathPrefix, fileSuffix;

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

	inline const std::string& getPathPrefix() const {
		return pathPrefix;
	}

	inline const std::string& getFileSuffix() const {
		return fileSuffix;
	}

	void usage();
	void setPathPrefix(const std::string&);
	void setFileSuffix(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_LNBLOB_OPTIONS_HPP */
