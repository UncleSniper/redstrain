#ifndef REDSTRAIN_MOD_BUILD_ONETOONETRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_ONETOONETRANSFORM_HPP

#include "Transform.hpp"

namespace redengine {
namespace build {

	template<typename ArtifactT>
	class OneToOneTransform : public Transform {

	  private:
		ArtifactT* source;

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
