#ifndef REDSTRAIN_MOD_REDMOND_LIBRARYDEPENDENCY_HPP
#define REDSTRAIN_MOD_REDMOND_LIBRARYDEPENDENCY_HPP

#include <string>

#include "constants.hpp"
#include "environment.hpp"

namespace redengine {
namespace redmond {

	class REDSTRAIN_REDMOND_API LibraryDependency {

	  private:
		const std::string outerComponent, innerComponent;
		const unsigned expectedMajor, expectedMinor, actualMajor, actualMinor;
		const CompilerFlavor expectedCompiler, actualCompiler;

	  public:
		LibraryDependency(const std::string&, const std::string&,
				unsigned, unsigned, CompilerFlavor, unsigned, unsigned, CompilerFlavor);
		LibraryDependency(const LibraryDependency&);

		inline const std::string& getOuterComponentName() const {
			return outerComponent;
		}

		inline const std::string& getInnerComponentName() const {
			return innerComponent;
		}

		inline unsigned getExpectedMajor() const {
			return expectedMajor;
		}

		inline unsigned getExpectedMinor() const {
			return expectedMinor;
		}

		inline CompilerFlavor getExpectedCompiler() const {
			return expectedCompiler;
		}

		inline unsigned getActualMajor() const {
			return actualMajor;
		}

		inline unsigned getActualMinor() const {
			return actualMinor;
		}

		inline CompilerFlavor getActualCompiler() const {
			return actualCompiler;
		}

		std::string getExpectedVersionSpecifier() const;
		std::string getActualVersionSpecifier() const;
		void checkVersion() const;

		static const char* formatCompilerFlavor(CompilerFlavor);

	};

}}

#if defined(REDSTRAIN_ENV_COMPILER_GNU)
#define REDSTRAIN_REDMOND_BUILD_COMPILER ::redengine::redmond::COMP_GCC
#elif defined(REDSTRAIN_ENV_COMPILER_MSVC)
#define REDSTRAIN_REDMOND_BUILD_COMPILER ::redengine::redmond::COMP_MSVC
#else /* unknown compiler */
#define REDSTRAIN_REDMOND_BUILD_COMPILER ::redengine::redmond::COMP_UNKNOWN
#endif /* build compiler switch */

#define REDSTRAIN_DECLARE_MODULE_VERSION(exportMacro) \
	exportMacro extern const unsigned dynamicVersionMajor; \
	exportMacro extern const unsigned dynamicVersionMinor; \
	exportMacro extern const ::redengine::redmond::CompilerFlavor dynamicVersionCompiler;

#define REDSTRAIN_DEFINE_MODULE_VERSION(versionMacroPrefix) \
	extern const unsigned dynamicVersionMajor = versionMacroPrefix ## _STATIC_VERSION_MAJOR; \
	extern const unsigned dynamicVersionMinor = versionMacroPrefix ## _STATIC_VERSION_MINOR; \
	extern const ::redengine::redmond::CompilerFlavor dynamicVersionCompiler = REDSTRAIN_REDMOND_BUILD_COMPILER;

#define REDSTRAIN_DEPEND_MODULE(varName, outerName, innerName, versionMacroPrefix, innerNamespace) \
	static ::redengine::redmond::LibraryDependency dependOn ## varName( \
		#outerName, #innerName, \
		versionMacroPrefix ## _STATIC_VERSION_MAJOR, \
		versionMacroPrefix ## _STATIC_VERSION_MINOR, \
		REDSTRAIN_REDMOND_BUILD_COMPILER, \
		innerNamespace::dynamicVersionMajor, \
		innerNamespace::dynamicVersionMinor, \
		innerNamespace::dynamicVersionCompiler \
	);

#endif /* REDSTRAIN_MOD_REDMOND_LIBRARYDEPENDENCY_HPP */
