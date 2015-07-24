#ifndef REDSTRAIN_MOD_BUILD_LINKGENERATION_HPP
#define REDSTRAIN_MOD_BUILD_LINKGENERATION_HPP

#include "Linker.hpp"
#include "Generation.hpp"
#include "FileArtifact.hpp"

namespace redengine {
namespace build {

	class LinkerConfiguration;

	class REDSTRAIN_BUILD_API LinkGeneration : public Generation<FileArtifact> {

	  private:
		Linker& linker;
		Linkage::LinkMode mode;
		LinkerConfiguration& configuration;

	  public:
		LinkGeneration(Linker&, Linkage::LinkMode, LinkerConfiguration&);
		LinkGeneration(const LinkGeneration&);

		inline Linker& getLinker() {
			return linker;
		}

		inline const Linker& getLinker() const {
			return linker;
		}

		inline Linkage::LinkMode getLinkMode() const {
			return mode;
		}

		inline void setLinkMode(Linkage::LinkMode mode) {
			this->mode = mode;
		}

		inline LinkerConfiguration& getLinkerConfiguration() {
			return configuration;
		}

		inline const LinkerConfiguration& getLinkerConfiguration() const {
			return configuration;
		}

		virtual void generate(const std::list<FileArtifact*>&, FileArtifact*, BuildContext&);
		virtual void notifyUIWillGenerate(BuildUI&, const Action&, const std::list<FileArtifact*>&, FileArtifact*);
		virtual void notifyUIWouldGenerate(BuildUI&, const Action&, const std::list<FileArtifact*>&, FileArtifact*);

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpGeneration(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKGENERATION_HPP */
