#ifndef REDSTRAIN_TOOLS_MKMSGS_OPTIONS_HPP
#define REDSTRAIN_TOOLS_MKMSGS_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string infile, outfile, specfile, inenc, specenc;

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

	inline const std::string& getSpecificationFile() const {
		return specfile;
	}

	inline const std::string& getInputFileEncoding() const {
		return inenc;
	}

	inline const std::string& getSpecificationFileEncoding() const {
		return specenc;
	}

	void usage();
	void setSpecificationFile(const std::string&);
	void setInputFileEncoding(const std::string&);
	void setSpecificationFileEncoding(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_MKMSGS_OPTIONS_HPP */
