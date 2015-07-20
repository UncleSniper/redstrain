#include "Resources.hpp"

using std::string;

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
		"skip.tool.install"
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

	const char* Resources::getResourceKey(ID id) {
		if(static_cast<int>(id) < 0 || id >= RES__LAST)
			return NULL;
		return KEYS[id];
	}

}}}
