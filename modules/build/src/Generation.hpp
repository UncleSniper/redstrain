#ifndef REDSTRAIN_MOD_BUILD_GENERATION_HPP
#define REDSTRAIN_MOD_BUILD_GENERATION_HPP

#include <list>
#include <redstrain/util/ReferenceCounted.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamtypes.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

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

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpGeneration(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GENERATION_HPP */
