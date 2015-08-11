#ifndef REDSTRAIN_MOD_BUILD_REMOVEGOAL_HPP
#define REDSTRAIN_MOD_BUILD_REMOVEGOAL_HPP

#include <redstrain/util/UniqueList.hpp>

#include "Goal.hpp"

namespace redengine {
namespace build {

	class Artifact;

	class REDSTRAIN_BUILD_API RemoveGoal : public Goal {

	  private:
		typedef util::UniqueList<Artifact*> Artifacts;

	  public:
		typedef Artifacts::Iterator ArtifactIterator;

	  private:
		Artifacts artifacts;

	  public:
		RemoveGoal();
		RemoveGoal(const RemoveGoal&);
		virtual ~RemoveGoal();

		bool addArtifact(Artifact&);
		bool removeArtifact(Artifact&);
		void clearArtifacts();
		void getArtifacts(ArtifactIterator&, ArtifactIterator&) const;

		virtual void attain(BuildContext&);
		virtual void wouldAttain(BuildContext&);

		virtual void dumpGoal(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_REMOVEGOAL_HPP */
