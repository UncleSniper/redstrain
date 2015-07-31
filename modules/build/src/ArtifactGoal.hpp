#ifndef REDSTRAIN_MOD_BUILD_ARTIFACTGOAL_HPP
#define REDSTRAIN_MOD_BUILD_ARTIFACTGOAL_HPP

#include <redstrain/util/UniqueList.hpp>

#include "Goal.hpp"

namespace redengine {
namespace build {

	class Artifact;

	class REDSTRAIN_BUILD_API ArtifactGoal : public Goal {

	  private:
		typedef util::UniqueList<Artifact*> Artifacts;

	  public:
		typedef Artifacts::Iterator ArtifactIterator;

	  private:
		Artifacts artifacts;

	  public:
		ArtifactGoal();
		ArtifactGoal(const ArtifactGoal&);
		virtual ~ArtifactGoal();

		bool addArtifact(Artifact&);
		bool removeArtifact(Artifact&);
		void clearArtifacts();
		void getArtifacts(ArtifactIterator&, ArtifactIterator&) const;

		virtual void attain(BuildContext&);
		virtual void wouldAttain(BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ARTIFACTGOAL_HPP */
