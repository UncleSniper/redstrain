#include "DefaultTransformPropertyInjector.hpp"

namespace redengine {
namespace build {

	DefaultTransformPropertyInjector::DefaultTransformPropertyInjector() {}

	DefaultTransformPropertyInjector::DefaultTransformPropertyInjector(const DefaultTransformPropertyInjector&
			injector) : TransformPropertyInjector(injector) {}

	void DefaultTransformPropertyInjector::injectTransformProperties(Component&,
			const Language&, const Flavor&, Transform&) {}

}}
