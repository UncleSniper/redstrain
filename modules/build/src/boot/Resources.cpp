#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "Resources.hpp"

using std::string;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::io::FileInputStream;
using redengine::error::IllegalArgumentError;

namespace redengine {
namespace build {
namespace boot {

	static const char *const KEYS[] = {
		"compiler.flavor",
		"compiler.binary",
		"dynamic.linker.binary",
		"static.linker.binary",
		"executable.linker.binary",
		"use.pic.flag",
		"extra.dynamic.compiler.flags",
		"extra.static.compiler.flags",
		"extra.executable.compiler.flags",
		"extra.compiler.flags",
		"extra.dynamic.linker.flags",
		"extra.static.linker.flags",
		"extra.executable.linker.flags",
		"extra.linker.flags",
		"warn.dynamic.compiler.flags",
		"warn.static.compiler.flags",
		"warn.executable.compiler.flags",
		"warn.compiler.flags",
		"warn.dynamic.linker.flags",
		"warn.static.linker.flags",
		"warn.executable.linker.flags",
		"warn.linker.flags",
		"libraries.directory",
		"binaries.directory",
		"common.modules.properties.file",
		"common.tools.properties.file",
		"component.properties.file",
		"dynamic.library.decoration",
		"static.library.decoration",
		"dynamic.executable.decoration",
		"static.executable.decoration",
		"object.file.decoration",
		"project.name",
		"project.guard",
		"module.basename",
		"tool.basename",
		"source.directory",
		"build.directory",
		"header.expose.directory",
		"static.build.directory",
		"dynamic.build.directory",
		"module.tag",
		"tool.tag",
		"dynamic.flavor.decoration",
		"static.flavor.decoration",
		"build.dynamic.libraries",
		"build.static.libraries",
		"build.dynamic.executables",
		"build.static.executables",
		"internal.api.macro",
		"external.include.directories",
		"external.library.directories",
		"external.libraries",
		"interlink.libraries",
		"install.prefix",
		"install.library.directory",
		"install.binary.directory",
		"install.header.directory",
		"make.installed.executable",
		"skip.header.install",
		"skip.library.install",
		"skip.tool.install",
		"component.name",
		"component.guard",
		"depend.modules",
		"target.architecture",
		"target.os",
		"blobs.directory",
		"generated.namespace",
		"export.macro",
		"blob.path",
		"generated.source.directory",
		"linux.external.libraries",
		"windows.external.libraries"
	};

	Resources::Resources() {}

	Resources::Resources(const Resources& resources) : properties(resources.properties) {}

	bool Resources::hasProperty(ID id) const {
		const char* key = Resources::getResourceKey(id);
		if(!key)
			return false;
		return properties.hasProperty(key);
	}

	string Resources::getProperty(ID id) const {
		const char* key = Resources::getResourceKey(id);
		if(!key)
			return "";
		return properties.getProperty(key);
	}

	void Resources::load(const string& path) {
		FileInputStream fis(path);
		StreamCloser closer(fis);
		properties.load(fis);
		closer.close();
	}

	void Resources::load(Defaults defaults) {
		switch(defaults) {
			case DFL_DEFAULTS:
				putDefaults();
				break;
			case DFL_LINUX_DEFAULTS:
				putLinuxDefaults();
				break;
			case DFL_WINDOWS_DEFAULTS:
				putWindowsDefaults();
				break;
			default:
				throw IllegalArgumentError("Unknown defaults ID: "
						+ StringUtils::toString(static_cast<int>(defaults)));
		}
	}

	const char* Resources::getResourceKey(ID id) {
		if(static_cast<int>(id) < 0 || id >= RES__LAST)
			return NULL;
		return KEYS[id];
	}

	void Resources::putBuiltin(const char *const* lines) {
		for(; *lines; ++lines) {
			string line(*lines);
			string::size_type pos = line.find('=');
			if(pos != string::npos)
				properties.setProperty(line.substr(static_cast<string::size_type>(0u), pos),
						line.substr(pos + static_cast<string::size_type>(1u)));
		}
	}

	static const char *const PROP_DEFAULTS[] = {
		"libraries.directory=modules",
		"binaries.directory=tools",
		"blobs.directory=data",
		"common.modules.properties.file=modules.properties",
		"common.tools.properties.file=tools.properties",
		"component.properties.file=component.properties",
		"module.basename=%project%-%module%",
		"tool.basename=%project%%tool%",
		"source.directory=src",
		"generated.source.directory=gensrc",
		"build.directory=build",
		"header.expose.directory=include",
		"static.build.directory=static",
		"dynamic.build.directory=dynamic",
		"module.tag=module",
		"tool.tag=tool",
		"static.flavor.decoration=%-static",
		"internal.api.macro=BUILDING_%project%_%module%_API",
		"install.library.directory=lib",
		"install.binary.directory=bin",
		"install.header.directory=include",
		"gcc.warn.compiler.flags=-pedantic -Wall -Wextra -Wno-long-long",
		"generated.namespace=%project%::%module%",
		"export.macro=%project%_%module%_API",
		"blob.path=/%project%/%module%/%srctail%",
		NULL
	};

	static const char *const PROP_LINUX_DEFAULTS[] = {
		"compiler.flavor=gcc",
		"gcc.dynamic.library.decoration=lib%.so",
		"gcc.static.library.decoration=lib%.a",
		"dynamic.executable.decoration=%",
		"static.executable.decoration=%-static",
		"gcc.object.file.decoration=%.o",
		"install.prefix=/usr",
		"make.installed.executable=true",
		NULL
	};

	static const char *const PROP_WINDOWS_DEFAULTS[] = {
		"compiler.flavor=gcc",
		"gcc.compiler.binary=x86_64-w64-mingw32-g++",
		"gcc.static.linker.binary=x86_64-w64-mingw32-gcc-ar",
		"gcc.dynamic.library.decoration=lib%.dll",
		"msvc.dynamic.library.decoration=%.dll",
		"gcc.static.library.decoration=lib%.a",
		"msvc.static.library.decoration=%.lib",
		"dynamic.executable.decoration=%.exe",
		"static.executable.decoration=%-static.exe",
		"gcc.object.file.decoration=%.o",
		"msvc.object.file.decoration=%.obj",
		"interlink.libraries=true",
		"install.prefix=dist",
		NULL
	};

	void Resources::putDefaults() {
		putBuiltin(PROP_DEFAULTS);
	}

	void Resources::putLinuxDefaults() {
		putBuiltin(PROP_LINUX_DEFAULTS);
	}

	void Resources::putWindowsDefaults() {
		putBuiltin(PROP_WINDOWS_DEFAULTS);
	}

}}}
