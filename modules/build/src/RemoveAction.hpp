#ifndef REDSTRAIN_MOD_BUILD_REMOVEACTION_HPP
#define REDSTRAIN_MOD_BUILD_REMOVEACTION_HPP

#include <set>

#include "Action.hpp"

namespace redengine {
namespace build {

	class Artifact;

	class REDSTRAIN_BUILD_API RemoveAction : public Action {

	  private:
		typedef std::set<Artifact*> Artifacts;

	  public:
		typedef Artifacts::const_iterator ArtifactIterator;

	  private:
		Artifacts artifacts;

	  public:
		RemoveAction();
		RemoveAction(const RemoveAction&);
		virtual ~RemoveAction();

		bool addArtifact(Artifact*);
		bool removeArtifact(Artifact*);
		void clearArtifacts();
		void getArtifacts(ArtifactIterator&, ArtifactIterator&) const;

		virtual void perform(BuildContext&);
		virtual void wouldPerform(BuildContext&);
		virtual void notifyUIWillPerform(BuildUI&) const;
		virtual void notifyUIWouldPerform(BuildUI&) const;

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpAction(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_REMOVEACTION_HPP */
