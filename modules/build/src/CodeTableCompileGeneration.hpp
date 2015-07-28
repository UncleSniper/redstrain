#ifndef REDSTRAIN_MOD_BUILD_CODETABLECOMPILEGENERATION_HPP
#define REDSTRAIN_MOD_BUILD_CODETABLECOMPILEGENERATION_HPP

#include "Generation.hpp"
#include "FileArtifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CodeTableCompileGeneration : public Generation<FileArtifact> {

	  private:
		static FileArtifact* getSoleSource(const std::list<FileArtifact*>&);

	  public:
		CodeTableCompileGeneration();
		CodeTableCompileGeneration(const CodeTableCompileGeneration&);

		virtual void generate(const std::list<FileArtifact*>&, FileArtifact*, BuildContext&);
		virtual void notifyUIWillGenerate(BuildUI&, const Action&, const std::list<FileArtifact*>&, FileArtifact*);
		virtual void notifyUIWouldGenerate(BuildUI&, const Action&, const std::list<FileArtifact*>&, FileArtifact*);

		virtual void dumpGeneration(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CODETABLECOMPILEGENERATION_HPP */
