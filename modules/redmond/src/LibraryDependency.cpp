#include <sstream>

#include "environment.hpp"

#if defined(REDSTRAIN_ENV_OS_WINDOWS)
#include <windows.h>
#else /* probably UNIX-like */
#include <unistd.h>
#endif /* OS-specific headers */

#include "LibraryDependency.hpp"

using std::string;
using std::stringstream;

namespace redengine {
namespace redmond {

	LibraryDependency::LibraryDependency(const string& outerComponent, const string& innerComponent,
			unsigned expectedMajor, unsigned expectedMinor, CompilerFlavor expectedCompiler,
			unsigned actualMajor, unsigned actualMinor, CompilerFlavor actualCompiler)
			: outerComponent(outerComponent), innerComponent(innerComponent),
			expectedMajor(expectedMajor), expectedMinor(expectedMinor),
			actualMajor(actualMajor), actualMinor(actualMinor),
			expectedCompiler(expectedCompiler), actualCompiler(actualCompiler) {
		checkVersion();
	}

	LibraryDependency::LibraryDependency(const LibraryDependency& dependency)
			: outerComponent(dependency.outerComponent), innerComponent(dependency.innerComponent),
			expectedMajor(dependency.expectedMajor), expectedMinor(dependency.expectedMinor),
			actualMajor(dependency.actualMajor), actualMinor(dependency.actualMinor),
			expectedCompiler(dependency.expectedCompiler), actualCompiler(dependency.actualCompiler) {}

	const char* LibraryDependency::formatCompilerFlavor(CompilerFlavor flavor) {
		switch(flavor) {
			case COMP_GCC:
				return "gcc";
			case COMP_MSVC:
				return "msvc";
			case COMP_UNKNOWN:
			default:
				return "unknown";
		}
	}

	string LibraryDependency::getExpectedVersionSpecifier() const {
		stringstream ss;
		ss << expectedMajor << '.' << expectedMinor << "-cabi-"
				<< LibraryDependency::formatCompilerFlavor(expectedCompiler);
		return ss.str();
	}

	string LibraryDependency::getActualVersionSpecifier() const {
		stringstream ss;
		ss << actualMajor << '.' << actualMinor << "-cabi-"
				<< LibraryDependency::formatCompilerFlavor(actualCompiler);
		return ss.str();
	}

	void LibraryDependency::checkVersion() const {
		string expected(getExpectedVersionSpecifier()), actual(getActualVersionSpecifier());
		if(expected == actual)
			return;
		stringstream ss;
		ss << "Component '" << outerComponent << "' expected version '" << expected
				<< "' of component '" << innerComponent << "', but found version '" << actual << '\'';
		string msg(ss.str());
		string caption("Software component version conflict");
#if defined(REDSTRAIN_ENV_OS_WINDOWS)
		MessageBoxA(NULL, msg.c_str(), caption.c_str(), MB_OK | MB_ICONERROR);
		ExitProcess(static_cast<UINT>(1u));
#else /* probably UNIX-like */
		caption.append(": ");
		write(2, caption.c_str(), static_cast<size_t>(caption.length()));
		write(2, msg.c_str(), static_cast<size_t>(msg.length()));
		write(2, "\n", static_cast<size_t>(1u));
		_exit(1);
#endif /* OS switch */
	}

}}
