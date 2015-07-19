#include <list>
#include <sstream>
#include <redstrain/util/StringUtils.hpp>

#include "Pathname.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <unistd.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <windows.h>
#endif /* OS-specific includes */

using std::list;
using std::string;
using std::stringstream;
using redengine::util::StringUtils;

namespace redengine {
namespace platform {

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
	const char *const Pathname::SEPARATOR = "/";
	const char *const Pathname::THIS_DIRECTORY = ".";
	const char *const Pathname::PARENT_DIRECTORY = "..";
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
	const char *const Pathname::SEPARATOR = "\\";
	const char *const Pathname::THIS_DIRECTORY = ".";
	const char *const Pathname::PARENT_DIRECTORY = "..";
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific constants */

	string Pathname::join(const string& parent, const string& child) {
		if(parent.empty())
			return child;
		if(child.empty())
			return parent;
		if(isAbsolute(child))
			return child;
		string croot, ctail;
		splitRoot(child, croot, ctail);
		if(isRooted(child) && isRooted(parent) && rootOf(parent) != croot)
			return child;
		bool psep = StringUtils::endsWith(parent, SEPARATOR);
		bool csep = StringUtils::startsWith(ctail, SEPARATOR);
		if(psep) {
			if(csep)
				return parent + ctail.substr(static_cast<string::size_type>(1u));
		}
		else {
			if(!csep)
				return parent + SEPARATOR + ctail;
		}
		return parent + ctail;
	}

	string Pathname::dirname(const string& pathname, NamingMode mode) {
		string root, path;
		splitRoot(pathname, root, path);
		string::size_type end = path.length(), pos = end;
		while(end && (pos = path.rfind(SEPARATOR, end - static_cast<string::size_type>(1u))) != string::npos) {
			if(end - pos > static_cast<string::size_type>(1u)) {
				if(mode == LOGICAL && path.substr(pos + static_cast<string::size_type>(1u),
						end - pos - static_cast<string::size_type>(1u)) == PARENT_DIRECTORY)
					return StringUtils::endsWith(path, SEPARATOR) ? pathname + PARENT_DIRECTORY
							: pathname + SEPARATOR + PARENT_DIRECTORY;
				return root + (pos ? path.substr(static_cast<string::size_type>(0u), pos) : SEPARATOR);
			}
			end = pos;
		}
		if(end)  // single segment
			return root + THIS_DIRECTORY;
		else if(path.empty())
			return root + PARENT_DIRECTORY;
		else  // only separators
			return root + SEPARATOR;
	}

	string Pathname::basename(const string& pathname) {
		string path(pathOf(pathname));
		string::size_type pos, end = path.length();
		while(end && (pos = path.rfind(SEPARATOR, end - static_cast<string::size_type>(1u))) != string::npos) {
			if(end - pos > static_cast<string::size_type>(1u))
				return path.substr(pos + static_cast<string::size_type>(1u),
						end - pos - static_cast<string::size_type>(1u));
			end = pos;
		}
		return end ? path : "";
	}

	string Pathname::tidy(const string& pathname) {
		bool abs = isAbsolute(pathname);
		string root, path;
		splitRoot(pathname, root, path);
		string::size_type pos, old = static_cast<string::size_type>(0u);
		list<string> segments;
		unsigned outside = 0u;
		while((pos = path.find(SEPARATOR, old)) != string::npos) {
			if(pos > old) {
				string seg(path.substr(old, pos - old));
				if(seg != THIS_DIRECTORY) {
					if(seg == PARENT_DIRECTORY) {
						if(segments.empty())
							++outside;
						else
							segments.pop_back();
					}
					else
						segments.push_back(seg);
				}
			}
			old = pos + static_cast<string::size_type>(1u);
		}
		if(old < path.length()) {
			string seg(path.substr(old));
			if(seg != THIS_DIRECTORY) {
				if(seg == PARENT_DIRECTORY) {
					if(segments.empty())
						++outside;
					else
						segments.pop_back();
				}
				else
					segments.push_back(seg);
			}
		}
		stringstream ss;
		ss << root;
		bool needSep = false;
		if(abs) {
			needSep = true;
			outside = 0u;
		}
		for(; outside; --outside) {
			if(needSep)
				ss << SEPARATOR;
			else
				needSep = true;
			ss << PARENT_DIRECTORY;
		}
		list<string>::const_iterator it;
		for(it = segments.begin(); it != segments.end(); ++it) {
			if(needSep)
				ss << SEPARATOR;
			else
				needSep = true;
			ss << *it;
		}
		if(static_cast<string::size_type>(ss.tellp()) == root.length())
			ss << THIS_DIRECTORY;
		return ss.str();
	}

	bool Pathname::isAbsolute(const string& pathname) {
		return StringUtils::startsWith(pathOf(pathname), SEPARATOR);
	}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	bool Pathname::isRooted(const string&) {
		return true;
	}

	void Pathname::splitRoot(const string& pathname, string& root, string& path) {
		root.clear();
		path = pathname;
	}

	string Pathname::rootOf(const string&) {
		return "";
	}

	string Pathname::pathOf(const string& pathname) {
		return pathname;
	}

	string Pathname::getWorkingDirectory() {
		size_t bufsize = static_cast<size_t>(0xF00u);
		char* buffer;
		for(;;) {
			buffer = new char[bufsize];
			if(getcwd(buffer, bufsize)) {
				string cwd(buffer);
				delete[] buffer;
				return cwd;
			}
			delete[] buffer;
			bufsize *= static_cast<size_t>(2u);
		}
	}

	string Pathname::canonicalizeSeparators(const string& pathname) {
		return pathname;
	}

	string Pathname::normalizeSeparators(const string& pathname) {
		return pathname;
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	bool Pathname::isRooted(const string& pathname) {
		if(pathname.length() < static_cast<string::size_type>(2u))
			return false;
		if(pathname[static_cast<string::size_type>(1u)] != ':')
			return false;
		char c = pathname[static_cast<string::size_type>(0u)];
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	void Pathname::splitRoot(const string& pathname, string& root, string& path) {
		if(isRooted(pathname)) {
			root = pathname.substr(static_cast<string::size_type>(0u), static_cast<string::size_type>(2u));
			char drive = root[static_cast<string::size_type>(0u)];
			if(drive >= 'a' && drive <= 'z')
				root[static_cast<string::size_type>(0u)] = static_cast<char>(drive - 'a' + 'A');
			path = pathname.substr(static_cast<string::size_type>(2u));
		}
		else {
			root.clear();
			path = pathname;
		}
	}

	string Pathname::rootOf(const string& pathname) {
		if(!isRooted(pathname))
			return "";
		char drive = pathname[static_cast<string::size_type>(0u)];
		if(drive >= 'A' && drive <= 'Z')
			return pathname.substr(static_cast<string::size_type>(0u), static_cast<string::size_type>(2u));
		string root;
		root += static_cast<char>(drive - 'a' + 'A');
		root += ':';
		return root;
	}

	string Pathname::pathOf(const string& pathname) {
		return isRooted(pathname) ? pathname.substr(static_cast<string::size_type>(2u)) : pathname;
	}

	string Pathname::getWorkingDirectory() {
		DWORD size = GetCurrentDirectory(static_cast<DWORD>(0u), NULL);
		char* buffer = new char[static_cast<size_t>(size)];
		GetCurrentDirectory(size, buffer);
		string cwd(buffer);
		delete[] buffer;
		return cwd;
	}

	string Pathname::canonicalizeSeparators(const string& pathname) {
		return StringUtils::replaceAll(pathname, "/", SEPARATOR);
	}

	string Pathname::normalizeSeparators(const string& pathname) {
		return StringUtils::replaceAll(pathname, SEPARATOR, "/");
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

	bool Pathname::startsWith(const string& haystack, const string& needle) {
		string n(needle);
		if(!StringUtils::endsWith(n, Pathname::SEPARATOR))
			n += Pathname::SEPARATOR;
		return StringUtils::startsWith(haystack, n) && haystack.length() > n.length();
	}

}}
