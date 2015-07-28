#ifndef REDSTRAIN_TOOLS_BUILD_OPTIONS_HPP
#define REDSTRAIN_TOOLS_BUILD_OPTIONS_HPP

#include <list>
#include <string>

class Options {

  private:
	typedef std::list<std::string> ValveNames;

  public:
	typedef ValveNames::const_iterator ValveNameIterator;

  private:
	const std::string progname;
	ValveNames valves;
	bool bootstrap, dry;
	std::string base;

  private:
	Options(const Options&);

  public:
	Options(const char*);

	inline bool isBootstrap() const {
		return bootstrap;
	}

	inline bool isDry() const {
		return dry;
	}

	inline const std::string& getBase() const {
		return base;
	}

	void getValves(ValveNameIterator&, ValveNameIterator&) const;

	void usage();
	void setBootstrap(bool);
	void setDry(bool);
	void setBase(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_BUILD_OPTIONS_HPP */
