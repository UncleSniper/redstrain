#ifndef REDSTRAIN_MOD_BUILD_GCC_HPP
#define REDSTRAIN_MOD_BUILD_GCC_HPP

#include "ExternalLinker.hpp"
#include "ExternalCompiler.hpp"
#include "PlatformConventions.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API GCC : public ExternalCompiler, public ExternalLinker {

	  private:
		class REDSTRAIN_BUILD_API GCCConventions : public PlatformConventions {

		  private:
			const PlatformConventions& platform;

		  public:
			GCCConventions(const PlatformConventions&);
			GCCConventions(const GCCConventions&);

			virtual std::string makeNativeExecutableName(const std::string&) const;
			virtual std::string makeNativeDynamicLibraryName(const std::string&) const;

		};

		class REDSTRAIN_BUILD_API GCCCompilation : public ExternalCompilation {

		  public:
			GCCCompilation(const std::string&, redmond::Architecture, const std::string&, CompileMode);
			GCCCompilation(const GCCCompilation&);

			virtual void defineMacro(const std::string&);
			virtual void defineMacro(const std::string&, const std::string&);
			virtual void addIncludeDirectory(const std::string&);
			virtual void invoke();

		};

		class REDSTRAIN_BUILD_API GCCLinkage : public ExternalLinkage {

		  public:
			GCCLinkage(const std::string&, redmond::Architecture, const std::string&, LinkMode);
			GCCLinkage(const GCCLinkage&);

			virtual void invoke();

		};

	  private:
		std::string arExecutable;

	  public:
		GCC(const std::string&, const std::string&, redmond::Architecture, redmond::OperatingSystem);
		GCC(const GCC&);

		inline const std::string& getARExecutable() const {
			return arExecutable;
		}

		void setARExecutable(const std::string&);

		virtual Compilation* newCompilation(const std::string&, Compilation::CompileMode);
		virtual std::string getObjectFileNameForSource(const std::string&);
		virtual Linkage* newLinkage(const std::string&, Linkage::LinkMode);
		virtual bool isObjectFile(const std::string&);
		virtual std::string decorateBinaryFileName(const std::string&, Linkage::LinkMode);
		virtual std::string getObjectFileFormatName();

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GCC_HPP */
