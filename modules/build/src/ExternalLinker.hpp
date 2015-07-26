#ifndef REDSTRAIN_MOD_BUILD_EXTERNALLINKER_HPP
#define REDSTRAIN_MOD_BUILD_EXTERNALLINKER_HPP

#include <list>

#include "Linker.hpp"
#include "ExternalTool.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ExternalLinker : public virtual ExternalTool, public Linker {

	  public:
		class REDSTRAIN_BUILD_API ExternalLinkage : public Linkage, public ExternalInvocation {

		  protected:
			std::list<std::string> libraryDirectories;
			std::list<std::string> libraries;

		  public:
			ExternalLinkage(const std::string&, const std::string&, LinkMode);
			ExternalLinkage(const ExternalLinkage&);

			virtual void addLibraryDirectory(const std::string&);
			virtual void addLibrary(const std::string&);

		};

	  public:
		ExternalLinker(const std::string&, redmond::Architecture, redmond::OperatingSystem);
		ExternalLinker(const ExternalLinker&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_EXTERNALLINKER_HPP */
