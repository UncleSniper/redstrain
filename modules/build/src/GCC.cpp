#include <cstring>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "GCC.hpp"
#include "platform.hpp"

using std::list;
using std::string;
using redengine::platform::Stat;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
using redengine::redmond::Architecture;
using redengine::redmond::OperatingSystem;
using redengine::redmond::OS_LINUX;
using redengine::redmond::ARCH_I686;
using redengine::redmond::OS_WINDOWS;
using redengine::redmond::ARCH_X86_64;

namespace redengine {
namespace build {

	// ======== GCCConventions ========

	GCC::GCCConventions::GCCConventions(const PlatformConventions& platform) : platform(platform) {}

	GCC::GCCConventions::GCCConventions(const GCCConventions& conventions)
			: PlatformConventions(conventions), platform(conventions.platform) {}

	string GCC::GCCConventions::makeNativeExecutableName(const string& basename) const {
		return platform.makeNativeExecutableName(basename);
	}

	string GCC::GCCConventions::makeNativeDynamicLibraryName(const string& basename) const {
		return "lib" + platform.makeNativeDynamicLibraryName(basename);
	}

	// ======== GCCCompilation ========

	GCC::GCCCompilation::GCCCompilation(const string& executable, Architecture architecture,
			const string& source, CompileMode mode) : ExternalCompilation(executable, source, mode) {
		command.addArgument("-c");
		switch(architecture) {
			case ARCH_I686:
				command.addArgument("-m32");
				break;
			case ARCH_X86_64:
				command.addArgument("-m64");
				break;
			default:
				break;
		}
		if(requiresPositionIndependentCode(architecture)) {
			command.addArgument("-fPIC");
			command.addArgument("-DPIC");
		}
	}

	GCC::GCCCompilation::GCCCompilation(const GCCCompilation& compilation)
			: Invocation(compilation), ExternalCompilation(compilation) {}

	void GCC::GCCCompilation::defineMacro(const string& name) {
		if(name.empty())
			return;
		command.addArgument("-D");
		command.addArgument(name);
	}

	void GCC::GCCCompilation::defineMacro(const string& name, const string& value) {
		if(name.empty())
			return;
		command.addArgument("-D");
		if(value.empty())
			command.addArgument(name);
		else
			command.addArgument(name + "=" + value);
	}

	void GCC::GCCCompilation::addIncludeDirectory(const string& path) {
		if(path.empty())
			return;
		command.addArgument("-I");
		command.addArgument(path);
	}

	void GCC::GCCCompilation::invoke() {
		command.addArgument("-o");
		command.addArgument(getTarget());
		command.addArgument(getSource());
		ExternalCompilation::invoke();
	}

	// ======== GCCLinkage ========

	GCC::GCCLinkage::GCCLinkage(const string& executable, Architecture architecture,
			const string& target, LinkMode mode) : ExternalLinkage(executable, target, mode) {
		switch(mode) {
			case STATIC_EXECUTABLE:
				command.addArgument("-static");
				command.addArgument("-Wl,-Bstatic");
				break;
			case DYNAMIC_LIBRARY:
				command.addArgument("-shared");
				command.addArgument("-Wl,-soname," + target);
				break;
			case STATIC_LIBRARY:
				command.addArgument("rcs");
				break;
			default:
				break;
		}
		if(mode != STATIC_LIBRARY) {
			switch(architecture) {
				case ARCH_I686:
					command.addArgument("-m32");
					break;
				case ARCH_X86_64:
					command.addArgument("-m64");
					break;
				default:
					break;
			}
		}
	}

	GCC::GCCLinkage::GCCLinkage(const GCCLinkage& linkage) : Invocation(linkage), ExternalLinkage(linkage) {}

	void GCC::GCCLinkage::invoke() {
		if(getLinkMode() == STATIC_LIBRARY) {
			list<string>::const_iterator lbegin(libraries.begin()), lend(libraries.end());
			for(; lbegin != lend; ++lbegin) {
				string lib("lib" + *lbegin + ".a");
				list<string>::const_iterator dbegin(libraryDirectories.begin()), dend(libraryDirectories.end());
				for(; dbegin != dend; ++dbegin) {
					string lpath(Pathname::join(*dbegin, lib));
					if(Filesystem::access(lpath, Filesystem::FILE_EXISTS)) {
						Stat info;
						Filesystem::stat(lpath, info);
						if(info.getType() == Stat::REGULAR_FILE) {
							command.addArgument(lpath);
							break;
						}
					}
				}
			}
		}
		else {
			list<string>::const_iterator begin(libraryDirectories.begin()), end(libraryDirectories.end());
			for(; begin != end; ++begin) {
				command.addArgument("-L");
				command.addArgument(*begin);
			}
			for(begin = libraries.begin(), end = libraries.end(); begin != end; ++begin) {
				command.addArgument("-l");
				command.addArgument(*begin);
			}
		}
		ExternalLinkage::invoke();
	}

	// ======== GCC ========

	GCC::GCC(const string& executable, const string& arExecutable, Architecture architecture,
			OperatingSystem targetOS) : ExternalTool(executable), ExternalCompiler(executable, architecture),
			ExternalLinker(executable, architecture, targetOS), arExecutable(arExecutable) {}

	GCC::GCC(const GCC& gcc) : ExternalTool(gcc), ExternalCompiler(gcc), ExternalLinker(gcc),
			arExecutable(gcc.arExecutable) {}

	void GCC::setARExecutable(const string& executable) {
		arExecutable = executable;
	}

	Compilation* GCC::newCompilation(const string& source, Compilation::CompileMode mode) {
		return new GCCCompilation(getExecutable(), ExternalCompiler::getTargetArchitecture(), source, mode);
	}

	static const char *const SOURCE_SUFFIXES[] = {
		".cpp",
		".cxx",
		NULL
	};

	string GCC::getObjectFileNameForSource(const string& source) {
		const char *const* suffix = SOURCE_SUFFIXES;
		for(; *suffix; ++suffix) {
			if(StringUtils::endsWith(source, *suffix))
				break;
		}
		if(*suffix)
			return source.substr(static_cast<string::size_type>(0u),
					source.length() - static_cast<string::size_type>(strlen(*suffix))) + ".o";
		else
			return source + ".o";
	}

	Linkage* GCC::newLinkage(const string& target, Linkage::LinkMode mode) {
		return new GCCLinkage(mode == Linkage::STATIC_LIBRARY ? arExecutable : getExecutable(),
				ExternalLinker::getTargetArchitecture(), target, mode);
	}

	bool GCC::isObjectFile(const string& path) {
		string bn(Pathname::basename(path));
		return bn.length() > static_cast<string::size_type>(2u) && StringUtils::endsWith(bn, ".o");
	}

	string GCC::decorateBinaryFileName(const string& name, Linkage::LinkMode mode) {
		switch(mode) {
			case Linkage::STATIC_EXECUTABLE:
			case Linkage::DYNAMIC_EXECUTABLE:
				return decorateNativeExecutableName(name, getTargetOperatingSystem());
			case Linkage::STATIC_LIBRARY:
				return "lib" + name + ".a";
			case Linkage::DYNAMIC_LIBRARY:
				return "lib" + decorateNativeDynamicLibraryName(name, getTargetOperatingSystem());
			default:
				return name;
		}
	}

	string GCC::getObjectFileFormatName() {
		Architecture arch = ExternalLinker::getTargetArchitecture();
		OperatingSystem os = getTargetOperatingSystem();
		switch(arch) {
			case ARCH_I686:
				switch(os) {
					case OS_LINUX:
						return "elf32-i386";
					case OS_WINDOWS:
						return "pe-i386";
					default:
						break;
				}
				break;
			case ARCH_X86_64:
				switch(os) {
					case OS_LINUX:
						return "elf64-x86-64";
					case OS_WINDOWS:
						return "pe-x86-64";
					default:
						break;
				}
				break;
			default:
				break;
		}
		return "object";
	}

}}
