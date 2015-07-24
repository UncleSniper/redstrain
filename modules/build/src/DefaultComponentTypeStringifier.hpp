#ifndef REDSTRAIN_MOD_BUILD_DEFAULTCOMPONENTTYPESTRINGIFIER_HPP
#define REDSTRAIN_MOD_BUILD_DEFAULTCOMPONENTTYPESTRINGIFIER_HPP

#include "Component.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API DefaultComponentTypeStringifier : public Component::ComponentTypeStringifier {

	  public:
		DefaultComponentTypeStringifier();
		DefaultComponentTypeStringifier(const DefaultComponentTypeStringifier&);

		virtual std::string stringifyComponentType(Component::Type);

	  public:
		static DefaultComponentTypeStringifier instance;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DEFAULTCOMPONENTTYPESTRINGIFIER_HPP */
