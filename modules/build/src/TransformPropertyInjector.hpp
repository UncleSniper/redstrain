#ifndef REDSTRAIN_MOD_BUILD_TRANSFORMPROPERTYINJECTOR_HPP
#define REDSTRAIN_MOD_BUILD_TRANSFORMPROPERTYINJECTOR_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Flavor;
	class Language;
	class Component;
	class Transform;

	class REDSTRAIN_BUILD_API TransformPropertyInjector {

	  public:
		TransformPropertyInjector();
		TransformPropertyInjector(const TransformPropertyInjector&);
		virtual ~TransformPropertyInjector();

		virtual void injectTransformProperties(Component&, const Language&, const Flavor&, Transform&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_TRANSFORMPROPERTYINJECTOR_HPP */
