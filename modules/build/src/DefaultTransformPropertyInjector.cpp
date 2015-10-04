#include "Transform.hpp"
#include "DefaultComponentTypeStringifier.hpp"
#include "DefaultTransformPropertyInjector.hpp"

namespace redengine {
namespace build {

	DefaultTransformPropertyInjector::DefaultTransformPropertyInjector(ComponentTypeStringifier* typeStringifier)
			: typeStringifier(typeStringifier) {}

	DefaultTransformPropertyInjector::DefaultTransformPropertyInjector(const DefaultTransformPropertyInjector&
			injector) : TransformPropertyInjector(injector), typeStringifier(injector.typeStringifier) {}

	void DefaultTransformPropertyInjector::injectTransformProperties(Component& component,
			const Language&, const Flavor&, Transform& transform) {
		if(typeStringifier)
			transform.setComponentType(typeStringifier->stringifyComponentType(component.getType()));
		else {
			switch(component.getType()) {
				#define clamp(c) \
					case Component::c: \
						transform.setComponentType(DefaultComponentTypeStringifier::DEFAULT_ ## c ## _TAG); \
						break;
				clamp(LIBRARY)
				clamp(EXECUTABLE)
				clamp(DATA)
				clamp(BLOB)
				#undef clamp
				default:
					break;
			}
		}
		transform.setComponentName(component.getName());
		transform.setComponentBaseDirectory(component.getBaseDirectory());
	}

}}
