#ifndef REDSTRAIN_MOD_BUILD_LINKEDLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_LINKEDLANGUAGE_HPP

#include "ObjectFileLanguage.hpp"

namespace redengine {
namespace build {

	class Linker;
	class LinkerConfiguration;

	class REDSTRAIN_BUILD_API LinkedLanguage : public ObjectFileLanguage {

	  private:
		Linker& linker;

	  protected:
		virtual LinkerConfiguration& getLinkerConfiguration(const Flavor&, const Component&);

	  public:
		LinkedLanguage(Linker&);
		LinkedLanguage(const LinkedLanguage&);

		inline Linker& getLinker() {
			return linker;
		}

		inline const Linker& getLinker() const {
			return linker;
		}

		virtual ArtifactType classifyFile(const std::string&);
		virtual bool acceptsSource(const Flavor&, const Flavor&);
		virtual FileArtifact* getSourceTransform(BuildContext&, const std::string&, FileArtifact&,
				const Flavor&, const std::string&, const Flavor&, Component&, BuildArtifactMapper&,
				ManyToOneTransform<FileArtifact>*&, Flavor&, bool&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKEDLANGUAGE_HPP */
