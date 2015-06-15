#ifndef REDSTRAIN_MOD_BUILD_GENERATIONACTION_HPP
#define REDSTRAIN_MOD_BUILD_GENERATIONACTION_HPP

#include "Action.hpp"
#include "Generation.hpp"

namespace redengine {
namespace build {

	template<typename ArtifactT>
	class GenerationAction : public Action {

	  private:
		typedef typename std::list<ArtifactT*> Artifacts;

	  public:
		typedef ArtifactT Artifact;
		typedef typename Artifacts::const_iterator ArtifactIterator;

	  private:
		struct ArtifactListUnref {

			Artifacts* artifacts;

			ArtifactListUnref(Artifacts* artifacts) : artifacts(artifacts) {}

			~ArtifactListUnref() {
				if(!artifacts)
					return;
				ArtifactIterator begin(artifacts->begin()), end(artifacts->end());
				for(; begin != end; ++begin)
					(*begin)->unref();
			}

		};

	  private:
		Generation<ArtifactT>& generation;
		Artifacts sources;
		ArtifactT* target;

	  public:
		GenerationAction(Generation<ArtifactT>& generation, ArtifactT* target = NULL)
				: generation(generation), target(target) {
			if(target)
				target->ref();
		}

		GenerationAction(const GenerationAction& action)
				: Action(action), generation(action.generation), target(action.target) {
			ArtifactListUnref unref(&sources);
			ArtifactIterator begin(action.sources.begin()), end(action.sources.end());
			for(; begin != end; ++begin) {
				sources.push_back(*begin);
				(*begin)->ref();
			}
			unref.artifacts = NULL;
		}

		virtual ~GenerationAction() {
			ArtifactIterator begin(sources.begin()), end(sources.end());
			for(; begin != end; ++begin)
				(*begin)->unref();
		}

		void addSource(ArtifactT* source) {
			if(!source)
				return;
			sources.push_back(source);
			source->ref();
		}

		bool removeSource(ArtifactT* source) {
			if(!source)
				return false;
			typename std::list<ArtifactT*>::iterator begin(sources.begin()), end(sources.end());
			for(; begin != end; ++begin) {
				if(*begin == source) {
					sources.erase(begin);
					source->unref();
					return true;
				}
			}
			return false;
		}

		void clearSources() {
			ArtifactIterator begin(sources.begin()), end(sources.end());
			for(; begin != end; ++begin)
				(*begin)->unref();
			sources.clear();
		}

		void getSources(ArtifactIterator& begin, ArtifactIterator& end) const {
			begin = sources.begin();
			end = sources.end();
		}

		virtual void perform(BuildContext& context) {
			generation.generate(sources, target, context);
		}

		virtual void wouldPerform(BuildContext&) {
			if(target)
				target->wouldModify();
		}

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GENERATIONACTION_HPP */
