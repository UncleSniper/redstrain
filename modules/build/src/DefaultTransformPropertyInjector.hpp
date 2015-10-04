#ifndef REDSTRAIN_MOD_BUILD_DEFAULTTRANSFORMPROPERTYINJECTOR_HPP
#define REDSTRAIN_MOD_BUILD_DEFAULTTRANSFORMPROPERTYINJECTOR_HPP

#include "TransformPropertyInjector.hpp"

namespace redengine {
namespace build {

	class ComponentTypeStringifier;

	class REDSTRAIN_BUILD_API DefaultTransformPropertyInjector : public TransformPropertyInjector {

	  private:
		ComponentTypeStringifier* typeStringifier;

	  public:
		DefaultTransformPropertyInjector(ComponentTypeStringifier* = NULL);
		DefaultTransformPropertyInjector(const DefaultTransformPropertyInjector&);

		inline ComponentTypeStringifier* getComponentTypeStringifier() const {
			return typeStringifier;
		}

		inline void setComponentTypeStringifier(ComponentTypeStringifier* typeStringifier) {
			this->typeStringifier = typeStringifier;
		}

		virtual void injectTransformProperties(Component&, const Language&, const Flavor&, Transform&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DEFAULTTRANSFORMPROPERTYINJECTOR_HPP */
