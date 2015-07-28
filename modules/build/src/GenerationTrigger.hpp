#ifndef REDSTRAIN_MOD_BUILD_GENERATIONTRIGGER_HPP
#define REDSTRAIN_MOD_BUILD_GENERATIONTRIGGER_HPP

#include "Trigger.hpp"
#include "Artifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API GenerationTrigger : public Trigger {

	  private:
		typedef std::list<Artifact*> Artifacts;

	  public:
		typedef Artifacts::const_iterator ArtifactIterator;

	  private:
		Artifacts sources, targets, optionalSources;

	  private:
		bool triggered(const Artifact::Mood&) const;

	  protected:
		virtual bool isTriggered(BuildContext&);
		virtual bool wouldTrigger(BuildContext&);

	  public:
		GenerationTrigger();
		GenerationTrigger(const GenerationTrigger&);
		virtual ~GenerationTrigger();

		void addSource(Artifact*);
		bool removeSource(Artifact*);
		void clearSources();
		void getSources(ArtifactIterator&, ArtifactIterator&) const;

		void addOptionalSource(Artifact*);
		bool removeOptionalSource(Artifact*);
		void clearOptionalSources();
		void getOptionalSources(ArtifactIterator&, ArtifactIterator&) const;

		void addTarget(Artifact*);
		bool removeTarget(Artifact*);
		void clearTargets();
		void getTargets(ArtifactIterator&, ArtifactIterator&) const;

		virtual void dumpTrigger(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GENERATIONTRIGGER_HPP */
