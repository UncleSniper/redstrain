#ifndef REDSTRAIN_TOOLS_MKBLOB_OPTIONS_HPP
#define REDSTRAIN_TOOLS_MKBLOB_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string infile, outfile, varname, expmacro, include, blob;
	bool header;

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

	inline const std::string& getVariableName() const {
		return varname;
	}

	inline const std::string& getExportMacro() const {
		return expmacro;
	}

	inline const std::string& getExtraInclude() const {
		return include;
	}

	inline bool shouldGenerateHeader() const {
		return header;
	}

	inline const std::string& getBlobPath() const {
		return blob;
	}

	void usage();
	void setVariableName(const std::string&);
	void setExportMacro(const std::string&);
	void setExtraInclude(const std::string&);
	void setGenerateHeader(bool);
	void setBlobPath(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_MKBLOB_OPTIONS_HPP */
