#ifndef REDSTRAIN_TOOLS_UPDMSGS_OPTIONS_HPP
#define REDSTRAIN_TOOLS_UPDMSGS_OPTIONS_HPP

#include <string>

class Options {

  private:
	unsigned barewordCount;
	const std::string progname;
	std::string specfile, msgfile, reffile, specenc, msgenc, refenc;
	bool verbose;

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

	inline const std::string& getReferenceFile() const {
		return reffile;
	}

	inline const std::string& getSpecificationFileEncoding() const {
		return specenc;
	}

	inline const std::string& getMessagesFileEncoding() const {
		return msgenc;
	}

	inline const std::string& getReferenceFileEncoding() const {
		return refenc;
	}

	inline bool isVerbose() const {
		return verbose;
	}

	void usage();
	void setReferenceFile(const std::string&);
	void setSpecificationFileEncoding(const std::string&);
	void setMessagesFileEncoding(const std::string&);
	void setReferenceFileEncoding(const std::string&);
	void setVerbose(bool);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_UPDMSGS_OPTIONS_HPP */
