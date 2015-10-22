#ifndef REDSTRAIN_TOOLS_UPDMSGS_OPTIONS_HPP
#define REDSTRAIN_TOOLS_UPDMSGS_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string specfile, msgfile, specenc, msgenc;

  private:
	Options(const Options&);

  public:
	Options(const char*);

	inline const std::string& getSpecificationFile() const {
		return specfile;
	}

	inline const std::string& getMessagesFile() const {
		return msgfile;
	}

	inline const std::string& getSpecificationFileEncoding() const {
		return specenc;
	}

	inline const std::string& getMessagesFileEncoding() const {
		return msgenc;
	}

	void usage();
	void setSpecificationFileEncoding(const std::string&);
	void setMessagesFileEncoding(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_UPDMSGS_OPTIONS_HPP */
