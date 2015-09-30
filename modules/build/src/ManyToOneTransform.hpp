#ifndef REDSTRAIN_MOD_BUILD_MANYTOONETRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_MANYTOONETRANSFORM_HPP

#include <redstrain/io/streamoperators.hpp>

#include "Transform.hpp"

namespace redengine {
namespace build {

	template<typename ArtifactT>
	class ManyToOneTransform : public Transform {

	  private:
		typedef util::UniqueList<ArtifactT*> Sources;

	  public:
		typedef typename Sources::Iterator SourceIterator;

	  private:
		Sources sources;

	  protected:
		void dumpManyToOneTransformAspects(io::DefaultConfiguredOutputStream<char>::Stream& stream) const {
			using redengine::io::endln;
			using redengine::io::shift;
			using redengine::io::indent;
			using redengine::io::unshift;
			using redengine::io::operator<<;
			stream << indent << "sources = {" << endln << shift;
			SourceIterator sbegin(sources.begin()), send(sources.end());
			for(; sbegin != send; ++sbegin)
				(*sbegin)->dumpArtifact(stream);
			stream << unshift << indent << '}' << endln;
		}

	  public:
		ManyToOneTransform() {}

		ManyToOneTransform(const ManyToOneTransform& transform) : Transform(transform), sources(transform.sources) {
			SourceIterator begin(sources.begin()), end(sources.end());
			for(; begin != end; ++begin)
				(*begin)->ref();
		}

		virtual ~ManyToOneTransform() {
			SourceIterator begin(sources.begin()), end(sources.end());
			for(; begin != end; ++begin)
				(*begin)->unref();
		}

		bool addSource(ArtifactT& source) {
			if(!sources.append(&source))
				return false;
			source.ref();
			return true;
		}

		bool removeSource(ArtifactT& source) {
			if(!sources.erase(&source))
				return false;
			source.unref();
			return true;
		}

		void clearSources() {
			SourceIterator begin(sources.begin()), end(sources.end());
			for(; begin != end; ++begin)
				(*begin)->unref();
			sources.clear();
		}

		void getSources(SourceIterator& begin, SourceIterator& end) const {
			begin = sources.begin();
			end = sources.end();
		}

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MANYTOONETRANSFORM_HPP */
