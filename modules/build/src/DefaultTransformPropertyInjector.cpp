#include "DefaultTransformPropertyInjector.hpp"

namespace redengine {
namespace build {

	DefaultTransformPropertyInjector::DefaultTransformPropertyInjector() {}

	DefaultTransformPropertyInjector::DefaultTransformPropertyInjector(const DefaultTransformPropertyInjector&
			injector) : TransformPropertyInjector(injector) {}

	void DefaultTransformPropertyInjector::injectSourceTransformProperties(Component&,
			const Language&, const Flavor&, Transform&) {}

	void DefaultTransformPropertyInjector::injectHeaderTransformProperties(Component&,
			const Language&, const Flavor&, Transform&) {}

}}
