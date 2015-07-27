#ifndef REDSTRAIN_MOD_BUILD_GENERATIONACTION_HPP
#define REDSTRAIN_MOD_BUILD_GENERATIONACTION_HPP

#include <cstddef>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "Action.hpp"
#include "BuildUI.hpp"
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

		inline Generation<ArtifactT>& getGeneration() {
			return generation;
		}

		inline const Generation<ArtifactT>& getGeneration() const {
			return generation;
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

		inline ArtifactT* getTarget() const {
			return target;
		}

		virtual void perform(BuildContext& context) {
			generation.generate(sources, target, context);
		}

		virtual void wouldPerform(BuildContext&) {
			if(target)
				target->wouldModify();
		}

		virtual void notifyUIWillPerform(BuildUI& ui) const {
			generation.notifyUIWillGenerate(ui, *this, sources, target);
		}

		virtual void notifyUIWouldPerform(BuildUI& ui) const {
			generation.notifyUIWouldGenerate(ui, *this, sources, target);
		}

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpAction(io::DefaultConfiguredOutputStream<char>::Stream& stream) const {
			using redengine::io::endln;
			using redengine::io::shift;
			using redengine::io::indent;
			using redengine::io::unshift;
			using redengine::io::operator<<;
			stream << indent << "GenerationAction " << this << " {" << endln << shift;
			stream << indent << "generation ->" << endln << shift;
			generation.dumpGeneration(stream);
			stream << unshift;
			stream << indent << "sources = {" << endln << shift;
			ArtifactIterator sbegin(sources.begin()), send(sources.end());
			for(; sbegin != send; ++sbegin)
				(*sbegin)->dumpArtifact(stream);
			stream << unshift << indent << '}' << endln;
			if(target) {
				stream << indent << "target ->" << endln << shift;
				target->dumpArtifact(stream);
				stream << unshift;
			}
			stream << unshift << indent << '}' << endln;
		}
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GENERATIONACTION_HPP */
