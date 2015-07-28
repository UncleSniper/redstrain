#ifndef REDSTRAIN_MOD_BUILD_GENERATION_HPP
#define REDSTRAIN_MOD_BUILD_GENERATION_HPP

#include <list>
#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/io/streamtypes.hpp>

namespace redengine {
namespace build {

	class Action;
	class BuildUI;
	class BuildContext;

	template<typename ArtifactT>
	class Generation : public util::ReferenceCounted {

	  public:
		typedef ArtifactT Artifact;

	  public:
		Generation() {}
		Generation(const Generation& generation) : ReferenceCounted(generation) {}
		virtual ~Generation() {}

		virtual void generate(const std::list<ArtifactT*>&, ArtifactT*, BuildContext&) = 0;
		virtual void notifyUIWillGenerate(BuildUI&, const Action&, const std::list<ArtifactT*>&, ArtifactT*) = 0;
		virtual void notifyUIWouldGenerate(BuildUI&, const Action&, const std::list<ArtifactT*>&, ArtifactT*) = 0;

		virtual void dumpGeneration(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GENERATION_HPP */
