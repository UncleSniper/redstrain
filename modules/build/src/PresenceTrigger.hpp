#ifndef REDSTRAIN_MOD_BUILD_PRESENCETRIGGER_HPP
#define REDSTRAIN_MOD_BUILD_PRESENCETRIGGER_HPP

#include "Trigger.hpp"
#include "Artifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API PresenceTrigger : public Trigger {

	  private:
		typedef std::set<Artifact*> Artifacts;

	  public:
		typedef Artifacts::const_iterator ArtifactIterator;

		enum REDSTRAIN_BUILD_API Junctor {
			ANY,
			ALL
		};

	  private:
		const Junctor junctor;
		Artifacts artifacts;

	  private:
		bool triggered(const Artifact::Mood&) const;

	  protected:
		virtual bool isTriggered(BuildContext&);
		virtual bool wouldTrigger(BuildContext&);

	  public:
		PresenceTrigger(Junctor);
		PresenceTrigger(const PresenceTrigger&);
		virtual ~PresenceTrigger();

		bool addArtifact(Artifact*);
		bool removeArtifact(Artifact*);
		void clearArtifacts();
		void getArtifacts(ArtifactIterator&, ArtifactIterator&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_PRESENCETRIGGER_HPP */
