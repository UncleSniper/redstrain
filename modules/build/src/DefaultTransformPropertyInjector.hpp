#ifndef REDSTRAIN_MOD_BUILD_DEFAULTTRANSFORMPROPERTYINJECTOR_HPP
#define REDSTRAIN_MOD_BUILD_DEFAULTTRANSFORMPROPERTYINJECTOR_HPP

#include "TransformPropertyInjector.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API DefaultTransformPropertyInjector : public TransformPropertyInjector {

	  public:
		DefaultTransformPropertyInjector();
		DefaultTransformPropertyInjector(const DefaultTransformPropertyInjector&);

		virtual void injectTransformProperties(Component&, const Language&, const Flavor&, Transform&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DEFAULTTRANSFORMPROPERTYINJECTOR_HPP */
