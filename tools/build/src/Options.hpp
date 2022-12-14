#ifndef REDSTRAIN_TOOLS_BUILD_OPTIONS_HPP
#define REDSTRAIN_TOOLS_BUILD_OPTIONS_HPP

#include <list>
#include <string>

class Options {

  private:
	typedef std::list<std::string> GoalNames;

  public:
	typedef GoalNames::const_iterator GoalNameIterator;

  private:
	const std::string progname;
	GoalNames goals;
	bool bootstrap, dry, dumpContext, showGoals;
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

	inline bool isDumpContext() const {
		return dumpContext;
	}

	inline bool isShowGoals() const {
		return showGoals;
	}

	inline const std::string& getBase() const {
		return base;
	}

	void getGoals(GoalNameIterator&, GoalNameIterator&) const;

	void usage();
	void setBootstrap(bool);
	void setDry(bool);
	void setDumpContext(bool);
	void setShowGoals(bool);
	void setBase(const std::string&);
	void addBareword(const std::string&);
	void checkBarewords();

};

#endif /* REDSTRAIN_TOOLS_BUILD_OPTIONS_HPP */
