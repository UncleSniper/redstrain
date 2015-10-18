#ifndef REDSTRAIN_TOOLS_MKGRISU_OPTIONS_HPP
#define REDSTRAIN_TOOLS_MKGRISU_OPTIONS_HPP

#include <list>
#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string outfile, ns, className, significantsName, exponentsName;

  private:
	Options(const Options&);

  public:
	Options(const char*);

	inline const std::string& getOutputFile() const {
		return outfile;
	}

	inline const std::string& getNamespace() const {
		return ns;
	}

	inline const std::string& getClassName() const {
		return className;
	}

	inline const std::string& getSignificantsName() const {
		return significantsName;
	}

	inline const std::string& getExponentsName() const {
		return exponentsName;
	}

	void usage();
	void setNamespace(const std::string&);
	void setClassName(const std::string&);
	void setSignificantsName(const std::string&);
	void setExponentsName(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_MKGRISU_OPTIONS_HPP */
