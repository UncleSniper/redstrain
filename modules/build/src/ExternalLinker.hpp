#ifndef REDSTRAIN_MOD_BUILD_EXTERNALLINKER_HPP
#define REDSTRAIN_MOD_BUILD_EXTERNALLINKER_HPP

#include "Linker.hpp"
#include "ExternalTool.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ExternalLinker : public virtual ExternalTool, public Linker {

	  public:
		class REDSTRAIN_BUILD_API ExternalLinkage : public Linkage, public ExternalInvocation {

		  public:
			ExternalLinkage(const std::string&, const std::string&, LinkMode);
			ExternalLinkage(const ExternalLinkage&);

		};

	  public:
		ExternalLinker(const std::string&, Architecture);
		ExternalLinker(const ExternalLinker&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_EXTERNALLINKER_HPP */
