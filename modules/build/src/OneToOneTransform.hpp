#ifndef REDSTRAIN_MOD_BUILD_ONETOONETRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_ONETOONETRANSFORM_HPP

#include <redstrain/io/streamoperators.hpp>

#include "Transform.hpp"

namespace redengine {
namespace build {

	template<typename ArtifactT>
	class OneToOneTransform : public Transform {

	  private:
		ArtifactT* source;

	  protected:
		void dumpOneToOneTransformAspects(io::DefaultConfiguredOutputStream<char>::Stream& stream) const {
			using redengine::io::endln;
			using redengine::io::shift;
			using redengine::io::indent;
			using redengine::io::unshift;
			using redengine::io::operator<<;
			stream << indent << "source =" << endln << shift;
			source->dumpArtifact(stream);
			stream << unshift;
		}

	  public:
		OneToOneTransform(ArtifactT& source) : source(&source) {
			source.ref();
		}

		OneToOneTransform(const OneToOneTransform& transform) : Transform(transform), source(transform.source) {
			source->ref();
		}

		virtual ~OneToOneTransform() {
			source->unref();
		}

		inline ArtifactT& getSource() const {
			return *source;
		}

		void setSource(ArtifactT& source) {
			source.ref();
			this->source->unref();
			this->source = &source;
		}

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ONETOONETRANSFORM_HPP */
