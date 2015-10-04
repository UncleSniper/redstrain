#ifndef REDSTRAIN_MOD_BUILD_COMPONENTTYPESTRINGIFIER_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTTYPESTRINGIFIER_HPP

#include "Component.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ComponentTypeStringifier {

	  public:
		ComponentTypeStringifier();
		ComponentTypeStringifier(const ComponentTypeStringifier&);
		virtual ~ComponentTypeStringifier();

		virtual std::string stringifyComponentType(Component::Type) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENTTYPESTRINGIFIER_HPP */
