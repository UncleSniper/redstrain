#ifndef REDSTRAIN_MOD_BUILD_BOOT_RESOURCES_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_RESOURCES_HPP

#include <redstrain/text/Properties.hpp>

#include "../api.hpp"

namespace redengine {
namespace build {
namespace boot {

	class REDSTRAIN_BUILD_API Resources {

	  public:
		enum REDSTRAIN_BUILD_API ID {
			RES_COMPILER_FLAVOR,
			RES_COMPILER_BINARY,
			RES_DYNAMIC_LINKER_BINARY,
			RES_STATIC_LINKER_BINARY,
			RES_EXECUTABLE_LINKER_BINARY,
			RES_USE_PIC_FLAG,
			RES_EXTRA_DYNAMIC_COMPILER_FLAGS,
			RES_EXTRA_STATIC_COMPILER_FLAGS,
			RES_EXTRA_EXECUTABLE_COMPILER_FLAGS,
			RES_EXTRA_COMPILER_FLAGS,
			RES_EXTRA_DYNAMIC_LINKER_FLAGS,
			RES_EXTRA_STATIC_LINKER_FLAGS,
			RES_EXTRA_EXECUTABLE_LINKER_FLAGS,
			RES_EXTRA_LINKER_FLAGS,
			RES_WARN_DYNAMIC_COMPILER_FLAGS,
			RES_WARN_STATIC_COMPILER_FLAGS,
			RES_WARN_EXECUTABLE_COMPILER_FLAGS,
			RES_WARN_COMPILER_FLAGS,
			RES_WARN_DYNAMIC_LINKER_FLAGS,
			RES_WARN_STATIC_LINKER_FLAGS,
			RES_WARN_EXECUTABLE_LINKER_FLAGS,
			RES_WARN_LINKER_FLAGS,
			RES_LIBRARIES_DIRECTORY,
			RES_BINARIES_DIRECTORY,
			RES_COMMON_MODULES_PROPERTIES_FILE,
			RES_COMMON_TOOLS_PROPERTIES_FILE,
			RES_COMPONENT_PROPERTIES_FILE,
			RES_DYNAMIC_LIBRARY_DECORATION,
			RES_STATIC_LIBRARY_DECORATION,
			RES_DYNAMIC_EXECUTABLE_DECORATION,
			RES_STATIC_EXECUTABLE_DECORATION,
			RES_OBJECT_FILE_DECORATION,
			RES_PROJECT_NAME,
			RES_PROJECT_GUARD,
			RES_MODULE_BASENAME,
			RES_TOOL_BASENAME,
			RES_SOURCE_DIRECTORY,
			RES_BUILD_DIRECTORY,
			RES_HEADER_EXPOSE_DIRECTORY,
			RES_STATIC_BUILD_DIRECTORY,
			RES_DYNAMIC_BUILD_DIRECTORY,
			RES_MODULE_TAG,
			RES_TOOL_TAG,
			RES_DYNAMIC_FLAVOR_DECORATION,
			RES_STATIC_FLAVOR_DECORATION,
			RES_BUILD_DYNAMIC_LIBRARIES,
			RES_BUILD_STATIC_LIBRARIES,
			RES_BUILD_DYNAMIC_EXECUTABLES,
			RES_BUILD_STATIC_EXECUTABLES,
			RES_INTERNAL_API_MACRO,
			RES_EXTERNAL_INCLUDE_DIRECTORIES,
			RES_EXTERNAL_LIBRARY_DIRECTORIES,
			RES_EXTERNAL_LIBRARIES,
			RES_INTERLINK_LIBRARIES,
			RES_INSTALL_PREFIX,
			RES_INSTALL_LIBRARY_DIRECTORY,
			RES_INSTALL_BINARY_DIRECTORY,
			RES_INSTALL_HEADER_DIRECTORY,
			RES_MAKE_INSTALLED_EXECUTABLE,
			RES_SKIP_HEADER_INSTALL,
			RES_SKIP_LIBRARY_INSTALL,
			RES_SKIP_TOOL_INSTALL,
			RES_COMPONENT_NAME,
			RES_COMPONENT_GUARD,
			RES_DEPEND_MODULES,
			RES_INCLUDE_DEBUG_SYMBOLS,
			RES_RSB_TARGET_ARCHITECTURE,
			RES_RSB_TARGET_OS,
			RES_RSB_BLOBS_DIRECTORY,
			RES_RSB_GENERATED_NAMESPACE,
			RES_RSB_EXPORT_MACRO,
			RES_RSB_BLOB_PATH,
			RES_RSB_BLOB_INCLUDE,
			RES_RSB_GUARD_MACRO,
			RES_RSB_GENERATED_SOURCE_DIRECTORY,
			RES_RSB_DATA_TAG,
			RES_RSB_BLOB_TAG,
			RES_RSB_STAGE_DIRECTORY,
			RES_RSB_BLOB_ALIAS_PATH_PREFIX,
			RES_RSB_BLOB_ALIAS_FILE_SUFFIX,
			RES_RSB_BLOBFUL_DEPEND_MODULES,
			RES_PFX_LINUX_EXTERNAL_LIBRARIES,
			RES_PFX_WINDOWS_EXTERNAL_LIBRARIES,
			RES__LAST
		};

		enum REDSTRAIN_BUILD_API Defaults {
			DFL_DEFAULTS,
			DFL_LINUX_DEFAULTS,
			DFL_WINDOWS_DEFAULTS
		};

	  private:
		text::Properties properties;

	  private:
		void putBuiltin(const char *const*);
		void putDefaults();
		void putLinuxDefaults();
		void putWindowsDefaults();

	  public:
		Resources();
		Resources(const Resources&);

		bool hasProperty(ID) const;
		std::string getProperty(ID) const;

		void load(const std::string&);
		void load(Defaults);

		static const char* getResourceKey(ID);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_RESOURCES_HPP */
