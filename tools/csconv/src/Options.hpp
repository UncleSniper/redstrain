#ifndef REDSTRAIN_TOOLS_CSCONV_OPTIONS_HPP
#define REDSTRAIN_TOOLS_CSCONV_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string inencoding, outencoding, infile, outfile;
	bool listCodecs;

  private:
	Options(const Options&);

  public:
	Options(const char*);

	inline const std::string& getInputEncoding() const {
		return inencoding;
	}

	inline const std::string& getOutputEncoding() const {
		return outencoding;
	}

	inline const std::string& getInputFile() const {
		return infile;
	}

	inline const std::string& getOutputFile() const {
		return outfile;
	}

	inline bool isListCodecs() const {
		return listCodecs;
	}

	inline void setListCodecs(bool listCodecs) {
		this->listCodecs = listCodecs;
	}

	void usage();
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_CSCONV_OPTIONS_HPP */
