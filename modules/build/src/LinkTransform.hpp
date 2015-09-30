#ifndef REDSTRAIN_MOD_BUILD_LINKTRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_LINKTRANSFORM_HPP

#include "Linkage.hpp"
#include "FileArtifact.hpp"
#include "ManyToOneTransform.hpp"

namespace redengine {
namespace build {

	class Linker;
	class LinkerConfiguration;

	class REDSTRAIN_BUILD_API LinkTransform : public ManyToOneTransform<FileArtifact> {

	  private:
		Linker& linker;
		Linkage::LinkMode mode;
		LinkerConfiguration& configuration;

	  public:
		LinkTransform(Linker&, Linkage::LinkMode, LinkerConfiguration&);
		LinkTransform(const LinkTransform&);

		inline Linker& getLinker() {
			return linker;
		}

		inline const Linker& getLinker() const {
			return linker;
		}

		inline Linkage::LinkMode getLinkMode() const {
			return mode;
		}

		inline LinkerConfiguration& getLinkerConfiguration() {
			return configuration;
		}

		inline const LinkerConfiguration& getLinkerConfiguration() const {
			return configuration;
		}

		virtual void perform(BuildContext&, Artifact&);

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKTRANSFORM_HPP */
